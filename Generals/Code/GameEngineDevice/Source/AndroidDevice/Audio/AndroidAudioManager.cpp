/*
**  AndroidAudioManager.cpp
**  Oboe-based AudioManager with PCM mixing for Android.
**
**  Audio lifecycle:
**    1. Game calls addAudioEvent() → base class queues AudioRequest
**    2. processRequestList() dispatches AR_Play → playAudioEvent()
**    3. playAudioEvent() loads WAV data and adds AndroidPlayingAudio to m_playingSources
**    4. onAudioReady() mixes all active sources into the Oboe output buffer
**    5. Finished sources move to m_stoppedSources → cleanupFinishedAudio()
*/

#include "PreRTS.h"

#include "AndroidDevice/Audio/AndroidAudioManager.h"
#include "Common/AsciiString.h"
#include "Common/AudioEventRTS.h"
#include "Common/AudioRequest.h"
#include "Common/AudioEventInfo.h"
#include "Common/FileSystem.h"
#include "Common/File.h"

#include <android/log.h>
#include <cstring>
#include <algorithm>

#define LOG_TAG "AndroidAudioManager"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static constexpr int OUTPUT_SAMPLE_RATE  = 44100;
static constexpr int OUTPUT_CHANNELS     = 2;  // stereo

// ===========================================================================
// Construction / Destruction
// ===========================================================================

AndroidAudioManager::AndroidAudioManager()
    : m_isStreamOpen(false)
{
    LOGI("AndroidAudioManager created");
}

AndroidAudioManager::~AndroidAudioManager()
{
    closeStream();

    // Cleanup any remaining sources
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        delete[] src->m_pcmData;
        if (src->m_cleanupEvent && src->m_audioEventRTS) {
            releaseAudioEventRTS(src->m_audioEventRTS);
        }
        delete src;
    }
    m_playingSources.clear();
    for (auto *src : m_stoppedSources) {
        delete[] src->m_pcmData;
        if (src->m_cleanupEvent && src->m_audioEventRTS) {
            releaseAudioEventRTS(src->m_audioEventRTS);
        }
        delete src;
    }
    m_stoppedSources.clear();

    LOGI("AndroidAudioManager destroyed");
}

// ===========================================================================
// SubsystemInterface overrides
// ===========================================================================

void AndroidAudioManager::init()
{
    AudioManager::init();
    openStream();
    LOGI("AndroidAudioManager::init() complete");
}

void AndroidAudioManager::reset()
{
    // Stop all playing audio
    {
        std::lock_guard<std::mutex> lock(m_audioMutex);
        for (auto *src : m_playingSources) {
            src->m_stopped = true;
        }
    }
    cleanupFinishedAudio();
    AudioManager::reset();
    LOGI("AndroidAudioManager::reset()");
}

void AndroidAudioManager::update()
{
    AudioManager::update();
    cleanupFinishedAudio();
}

// ===========================================================================
// Request Processing (mirrors Miles pattern)
// ===========================================================================

void AndroidAudioManager::processRequestList(void)
{
    for (auto it = m_audioRequests.begin(); it != m_audioRequests.end(); ) {
        AudioRequest *req = *it;
        if (!req) {
            it = m_audioRequests.erase(it);
            continue;
        }

        switch (req->m_request) {
            case AR_Play:
                if (req->m_usePendingEvent && req->m_pendingEvent) {
                    playAudioEvent(req->m_pendingEvent);
                    req->m_pendingEvent = nullptr; // ownership transferred
                }
                break;
            case AR_Stop:
                stopAudioEvent(req->m_handleToInteractOn);
                break;
            case AR_Pause:
                // Pause the audio with the given handle
                {
                    std::lock_guard<std::mutex> lock(m_audioMutex);
                    for (auto *src : m_playingSources) {
                        if (src->m_handle == req->m_handleToInteractOn) {
                            src->m_paused = true;
                        }
                    }
                }
                break;
        }

        releaseAudioRequest(req);
        it = m_audioRequests.erase(it);
    }
}

// ===========================================================================
// WAV File Loading
// ===========================================================================

// Simple WAV header parser — loads 16-bit PCM data
int16_t *AndroidAudioManager::loadWavFile(const char *filename,
                                           uint32_t &outSampleCount,
                                           int &outChannels,
                                           int &outSampleRate)
{
    outSampleCount = 0;
    outChannels    = 1;
    outSampleRate  = 44100;

    // Try to open via the game's file system
    File *file = TheFileSystem->openFile(filename, File::READ | File::BINARY);
    if (!file) {
        LOGW("loadWavFile: cannot open '%s'", filename);
        return nullptr;
    }

    // Read entire file into memory
    Int fileSize = file->seek(0, File::END);
    file->seek(0, File::START);

    if (fileSize < 44) {
        LOGW("loadWavFile: '%s' too small (%d bytes)", filename, fileSize);
        file->close();
        return nullptr;
    }

    uint8_t *rawData = new uint8_t[fileSize];
    file->read(rawData, fileSize);
    file->close();

    // Parse WAV header
    // Check RIFF header
    if (memcmp(rawData, "RIFF", 4) != 0 || memcmp(rawData + 8, "WAVE", 4) != 0) {
        LOGW("loadWavFile: '%s' not a valid WAV", filename);
        delete[] rawData;
        return nullptr;
    }

    // Find fmt chunk
    int16_t audioFormat = 0;
    int16_t numChannels = 1;
    int32_t sampleRate  = 44100;
    int16_t bitsPerSample = 16;
    uint32_t dataSize   = 0;
    uint8_t *dataStart  = nullptr;

    uint32_t pos = 12;
    while (pos + 8 <= (uint32_t)fileSize) {
        char chunkId[5] = {};
        memcpy(chunkId, rawData + pos, 4);
        uint32_t chunkSize = *(uint32_t*)(rawData + pos + 4);

        if (memcmp(chunkId, "fmt ", 4) == 0 && pos + 8 + chunkSize <= (uint32_t)fileSize) {
            audioFormat   = *(int16_t*)(rawData + pos + 8);
            numChannels   = *(int16_t*)(rawData + pos + 10);
            sampleRate    = *(int32_t*)(rawData + pos + 12);
            bitsPerSample = *(int16_t*)(rawData + pos + 22);
        } else if (memcmp(chunkId, "data", 4) == 0) {
            dataSize  = chunkSize;
            dataStart = rawData + pos + 8;
        }

        pos += 8 + chunkSize;
        if (chunkSize % 2 != 0) pos++; // WAV chunks are word-aligned
    }

    if (!dataStart || dataSize == 0) {
        LOGW("loadWavFile: '%s' no data chunk", filename);
        delete[] rawData;
        return nullptr;
    }

    if (audioFormat != 1) { // PCM = 1
        LOGW("loadWavFile: '%s' compressed (format=%d), skipping", filename, audioFormat);
        delete[] rawData;
        return nullptr;
    }

    if (bitsPerSample != 16) {
        LOGW("loadWavFile: '%s' unsupported bits/sample=%d", filename, bitsPerSample);
        delete[] rawData;
        return nullptr;
    }

    uint32_t totalSamples = dataSize / sizeof(int16_t);
    int16_t *pcmOut = new int16_t[totalSamples];
    memcpy(pcmOut, dataStart, totalSamples * sizeof(int16_t));

    outSampleCount = totalSamples;
    outChannels    = numChannels;
    outSampleRate  = sampleRate;

    delete[] rawData;

    LOGI("loadWavFile: '%s' loaded — %u samples, %d ch, %d Hz",
         filename, totalSamples, numChannels, sampleRate);
    return pcmOut;
}

// ===========================================================================
// Audio Playback
// ===========================================================================

void AndroidAudioManager::playAudioEvent(AudioEventRTS *event)
{
    if (!event) return;

    // Get the event info for the filename
    getInfoForAudioEvent(event);
    const AudioEventInfo *info = event->getAudioEventInfo();

    if (!info) {
        LOGW("playAudioEvent: no info for '%s'", event->getEventName().str());
        return;
    }

    // Pick a filename from the event's sound list
    const std::vector<AsciiString> &soundFiles = info->m_sounds;

    if (soundFiles.empty()) {
        LOGW("playAudioEvent: no sounds for '%s'", event->getEventName().str());
        return;
    }

    // Pick a random sound from the list (or first)
    int idx = (soundFiles.size() > 1) ? (rand() % (int)soundFiles.size()) : 0;
    const char *filename = soundFiles[idx].str();

    // Load PCM data
    uint32_t sampleCount = 0;
    int channels = 1, sampleRate = 44100;
    int16_t *pcmData = loadWavFile(filename, sampleCount, channels, sampleRate);

    if (!pcmData || sampleCount == 0) {
        LOGW("playAudioEvent: failed to load '%s'", filename);
        return;
    }

    // Create playing source
    AndroidPlayingAudio *src = new AndroidPlayingAudio();
    src->m_audioEventRTS = event;
    src->m_handle       = allocateNewHandle();
    src->m_pcmData      = pcmData;
    src->m_pcmSize      = sampleCount;
    src->m_pcmPos       = 0;
    src->m_channels     = channels;
    src->m_sampleRate   = sampleRate;
    src->m_looping      = info->m_loopCount != 1; // loop if not single-play
    src->m_volume       = event->getVolume();

    // Set the handle on the event for tracking
    event->setPlayingHandle(src->m_handle);

    {
        std::lock_guard<std::mutex> lock(m_audioMutex);
        m_playingSources.push_back(src);
    }

    LOGI("playAudioEvent: '%s' -> handle %u, %u samples, vol=%.2f",
         event->getEventName().str(), src->m_handle, sampleCount, src->m_volume);
}

void AndroidAudioManager::stopAudioEvent(AudioHandle handle)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        if (src->m_handle == handle) {
            src->m_stopped = true;
            return;
        }
    }
}

void AndroidAudioManager::cleanupFinishedAudio()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);

    // Move finished/stopped sources to stopped list
    auto it = m_playingSources.begin();
    while (it != m_playingSources.end()) {
        AndroidPlayingAudio *src = *it;
        if (src->m_stopped || src->isFinished()) {
            m_stoppedSources.push_back(src);
            it = m_playingSources.erase(it);
        } else {
            ++it;
        }
    }

    // Free stopped sources
    for (auto *src : m_stoppedSources) {
        delete[] src->m_pcmData;
        src->m_pcmData = nullptr;
        if (src->m_cleanupEvent && src->m_audioEventRTS) {
            releaseAudioEventRTS(src->m_audioEventRTS);
        }
        delete src;
    }
    m_stoppedSources.clear();
}

// ===========================================================================
// Oboe Stream Management
// ===========================================================================

void AndroidAudioManager::openStream()
{
    if (m_isStreamOpen) return;

    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Output)
           ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
           ->setSharingMode(oboe::SharingMode::Exclusive)
           ->setFormat(oboe::AudioFormat::I16)
           ->setChannelCount(OUTPUT_CHANNELS)
           ->setSampleRate(OUTPUT_SAMPLE_RATE)
           ->setDataCallback(this)
           ->setErrorCallback(this);

    oboe::Result result = builder.openStream(m_stream);
    if (result != oboe::Result::OK) {
        LOGE("Failed to open Oboe stream: %s", oboe::convertToText(result));
        return;
    }

    result = m_stream->requestStart();
    if (result != oboe::Result::OK) {
        LOGE("Failed to start Oboe stream: %s", oboe::convertToText(result));
        m_stream->close();
        m_stream.reset();
        return;
    }

    m_isStreamOpen = true;
    LOGI("Oboe stream opened: sampleRate=%d, channelCount=%d, framesPerBurst=%d",
         m_stream->getSampleRate(),
         m_stream->getChannelCount(),
         m_stream->getFramesPerBurst());
}

void AndroidAudioManager::closeStream()
{
    if (!m_isStreamOpen || !m_stream) return;

    m_stream->requestStop();
    m_stream->close();
    m_stream.reset();
    m_isStreamOpen = false;
    LOGI("Oboe stream closed");
}

// ===========================================================================
// Oboe Callbacks — PCM Mixing
// ===========================================================================

oboe::DataCallbackResult AndroidAudioManager::onAudioReady(
    oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    int16_t *output = static_cast<int16_t*>(audioData);
    const int outChannels = oboeStream->getChannelCount();
    const int totalOutSamples = numFrames * outChannels;

    // Ensure mix buffer is large enough
    if ((int)m_mixBuffer.size() < totalOutSamples) {
        m_mixBuffer.resize(totalOutSamples);
    }

    // Clear mix buffer
    std::fill(m_mixBuffer.begin(), m_mixBuffer.begin() + totalOutSamples, 0);

    // Lock and mix all active sources
    {
        std::lock_guard<std::mutex> lock(m_audioMutex);

        for (auto *src : m_playingSources) {
            if (src->m_stopped || src->m_paused || !src->m_pcmData) continue;

            float vol = src->m_volume;
            int srcCh = src->m_channels;

            for (int frame = 0; frame < numFrames; ++frame) {
                if (src->m_pcmPos >= src->m_pcmSize) {
                    if (src->m_looping) {
                        src->m_pcmPos = 0;
                    } else {
                        break; // done
                    }
                }

                // Read source sample(s)
                int16_t left = 0, right = 0;
                if (srcCh == 1) {
                    // Mono: duplicate to both channels
                    left = right = src->m_pcmData[src->m_pcmPos++];
                } else if (srcCh == 2) {
                    left  = src->m_pcmData[src->m_pcmPos++];
                    if (src->m_pcmPos < src->m_pcmSize) {
                        right = src->m_pcmData[src->m_pcmPos++];
                    }
                } else {
                    // Multi-channel: just take first two
                    left  = src->m_pcmData[src->m_pcmPos++];
                    if (src->m_pcmPos < src->m_pcmSize) {
                        right = src->m_pcmData[src->m_pcmPos++];
                    }
                    // Skip remaining channels
                    for (int c = 2; c < srcCh && src->m_pcmPos < src->m_pcmSize; ++c) {
                        src->m_pcmPos++;
                    }
                }

                // Apply volume and accumulate into mix buffer (32-bit to avoid clipping)
                int outIdx = frame * outChannels;
                m_mixBuffer[outIdx]     += (int32_t)(left  * vol);
                if (outChannels >= 2) {
                    m_mixBuffer[outIdx + 1] += (int32_t)(right * vol);
                }
            }
        }
    }

    // Clamp and write to output
    for (int i = 0; i < totalOutSamples; ++i) {
        int32_t sample = m_mixBuffer[i];
        if (sample > 32767)  sample = 32767;
        if (sample < -32768) sample = -32768;
        output[i] = (int16_t)sample;
    }

    return oboe::DataCallbackResult::Continue;
}

void AndroidAudioManager::onErrorAfterClose(
    oboe::AudioStream *oboeStream, oboe::Result error)
{
    LOGE("Oboe stream error: %s — attempting to reopen", oboe::convertToText(error));
    m_isStreamOpen = false;
    openStream();
}

// ===========================================================================
// AudioManager — Audio control
// ===========================================================================

void AndroidAudioManager::stopAudio(AudioAffect which)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        src->m_stopped = true;
    }
}

void AndroidAudioManager::pauseAudio(AudioAffect which)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        src->m_paused = true;
    }
}

void AndroidAudioManager::resumeAudio(AudioAffect which)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        src->m_paused = false;
    }
}

void AndroidAudioManager::pauseAmbient(Bool shouldPause)
{
    // Ambient audio treated same as regular for now
}

void AndroidAudioManager::killAudioEventImmediately(AudioHandle audioEvent)
{
    stopAudioEvent(audioEvent);
}

// ===========================================================================
// AudioManager — Music control (stub — will be expanded)
// ===========================================================================

void AndroidAudioManager::nextMusicTrack(void) {}
void AndroidAudioManager::prevMusicTrack(void) {}
Bool AndroidAudioManager::isMusicPlaying(void) const { return FALSE; }
Bool AndroidAudioManager::hasMusicTrackCompleted(const AsciiString& trackName, Int numberOfTimes) const { return FALSE; }
AsciiString AndroidAudioManager::getMusicTrackName(void) const { return AsciiString::TheEmptyString; }

// ===========================================================================
// AudioManager — Device management
// ===========================================================================

void AndroidAudioManager::openDevice(void) { openStream(); }
void AndroidAudioManager::closeDevice(void) { closeStream(); }
void *AndroidAudioManager::getDevice(void) { return nullptr; }

void AndroidAudioManager::notifyOfAudioCompletion(UnsignedInt audioCompleted, UnsignedInt flags) {}

// ===========================================================================
// AudioManager — Provider (Miles-specific concept, simplified for Oboe)
// ===========================================================================

UnsignedInt AndroidAudioManager::getProviderCount(void) const { return 1; }
AsciiString AndroidAudioManager::getProviderName(UnsignedInt providerNum) const { return "Oboe"; }
UnsignedInt AndroidAudioManager::getProviderIndex(AsciiString providerName) const { return 0; }
void AndroidAudioManager::selectProvider(UnsignedInt providerNdx) {}
void AndroidAudioManager::unselectProvider(void) {}
UnsignedInt AndroidAudioManager::getSelectedProvider(void) const { return 0; }
void AndroidAudioManager::setSpeakerType(UnsignedInt speakerType) {}
UnsignedInt AndroidAudioManager::getSpeakerType(void) { return 0; }

// ===========================================================================
// AudioManager — Channels
// ===========================================================================

UnsignedInt AndroidAudioManager::getNum2DSamples(void) const { return 16; }
UnsignedInt AndroidAudioManager::getNum3DSamples(void) const { return 16; }
UnsignedInt AndroidAudioManager::getNumStreams(void) const { return 4; }

// ===========================================================================
// AudioManager — Sound prioritization
// ===========================================================================

Bool AndroidAudioManager::doesViolateLimit(AudioEventRTS *event) const { return FALSE; }
Bool AndroidAudioManager::isPlayingLowerPriority(AudioEventRTS *event) const { return FALSE; }

Bool AndroidAudioManager::isPlayingAlready(AudioEventRTS *event) const
{
    std::lock_guard<std::mutex> lock(const_cast<AndroidAudioManager*>(this)->m_audioMutex);
    for (auto *src : m_playingSources) {
        if (src->m_audioEventRTS && !src->m_stopped &&
            src->m_audioEventRTS->getEventName() == event->getEventName()) {
            return TRUE;
        }
    }
    return FALSE;
}

Bool AndroidAudioManager::isObjectPlayingVoice(UnsignedInt objID) const { return FALSE; }

// ===========================================================================
// AudioManager — Volume & playback
// ===========================================================================

void AndroidAudioManager::adjustVolumeOfPlayingAudio(AsciiString eventName, Real newVolume)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        if (src->m_audioEventRTS &&
            src->m_audioEventRTS->getEventName() == eventName) {
            src->m_volume = newVolume;
        }
    }
}

void AndroidAudioManager::removePlayingAudio(AsciiString eventName)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (auto *src : m_playingSources) {
        if (src->m_audioEventRTS &&
            src->m_audioEventRTS->getEventName() == eventName) {
            src->m_stopped = true;
        }
    }
}

void AndroidAudioManager::removeAllDisabledAudio() {}

Bool AndroidAudioManager::has3DSensitiveStreamsPlaying(void) const { return FALSE; }

void *AndroidAudioManager::getHandleForBink(void) { return nullptr; }
void AndroidAudioManager::releaseHandleForBink(void) {}

void AndroidAudioManager::friend_forcePlayAudioEventRTS(const AudioEventRTS *eventToPlay)
{
    if (eventToPlay) {
        AudioEventRTS *copy = new AudioEventRTS(*eventToPlay);
        playAudioEvent(copy);
    }
}

void AndroidAudioManager::setPreferredProvider(AsciiString providerNdx) {}
void AndroidAudioManager::setPreferredSpeaker(AsciiString speakerType) {}

Real AndroidAudioManager::getFileLengthMS(AsciiString strToLoad) const { return -1.0f; }

void AndroidAudioManager::closeAnySamplesUsingFile(const void *fileToClose) {}

void AndroidAudioManager::setDeviceListenerPosition(void) {}
