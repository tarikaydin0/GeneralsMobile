/*
**  AndroidAudioManager.h
**  Oboe-based AudioManager implementation for Android with PCM mixing.
*/

#pragma once

#include "Common/GameAudio.h"
#include "Common/AsciiString.h"
#include <oboe/Oboe.h>
#include <vector>
#include <mutex>
#include <cstdint>

// ---------------------------------------------------------------------------
// AndroidPlayingAudio — represents a single audio source being mixed
// ---------------------------------------------------------------------------
struct AndroidPlayingAudio
{
    AudioEventRTS *m_audioEventRTS = nullptr;
    AudioHandle    m_handle        = 0;

    // PCM data (16-bit signed, mono or stereo, 44100 Hz)
    int16_t       *m_pcmData       = nullptr;
    uint32_t       m_pcmSize       = 0;   // total samples (not bytes)
    uint32_t       m_pcmPos        = 0;   // current playback position (samples)
    int            m_channels      = 1;   // 1=mono, 2=stereo
    int            m_sampleRate    = 44100;

    float          m_volume        = 1.0f;
    bool           m_looping       = false;
    bool           m_stopped       = false;
    bool           m_paused        = false;
    bool           m_cleanupEvent  = true;

    bool isFinished() const { return !m_looping && m_pcmPos >= m_pcmSize; }
};

// ---------------------------------------------------------------------------
// AndroidAudioManager
// ---------------------------------------------------------------------------
class AndroidAudioManager : public AudioManager,
                            public oboe::AudioStreamDataCallback,
                            public oboe::AudioStreamErrorCallback
{
public:
    AndroidAudioManager();
    virtual ~AndroidAudioManager();

    // --- SubsystemInterface ---
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;

    // --- AudioManager pure virtuals ---
#if defined(RTS_DEBUG)
    virtual void audioDebugDisplay(DebugDisplayInterface *dd, void *userData, FILE *fp = nullptr) override {}
#endif

    virtual void stopAudio(AudioAffect which) override;
    virtual void pauseAudio(AudioAffect which) override;
    virtual void resumeAudio(AudioAffect which) override;
    virtual void pauseAmbient(Bool shouldPause) override;

    virtual void killAudioEventImmediately(AudioHandle audioEvent) override;

    virtual void nextMusicTrack(void) override;
    virtual void prevMusicTrack(void) override;
    virtual Bool isMusicPlaying(void) const override;
    virtual Bool hasMusicTrackCompleted(const AsciiString& trackName, Int numberOfTimes) const override;
    virtual AsciiString getMusicTrackName(void) const override;

    virtual void openDevice(void) override;
    virtual void closeDevice(void) override;
    virtual void *getDevice(void) override;

    virtual void notifyOfAudioCompletion(UnsignedInt audioCompleted, UnsignedInt flags) override;

    virtual UnsignedInt getProviderCount(void) const override;
    virtual AsciiString getProviderName(UnsignedInt providerNum) const override;
    virtual UnsignedInt getProviderIndex(AsciiString providerName) const override;
    virtual void selectProvider(UnsignedInt providerNdx) override;
    virtual void unselectProvider(void) override;
    virtual UnsignedInt getSelectedProvider(void) const override;
    virtual void setSpeakerType(UnsignedInt speakerType) override;
    virtual UnsignedInt getSpeakerType(void) override;

    virtual UnsignedInt getNum2DSamples(void) const override;
    virtual UnsignedInt getNum3DSamples(void) const override;
    virtual UnsignedInt getNumStreams(void) const override;

    virtual Bool doesViolateLimit(AudioEventRTS *event) const override;
    virtual Bool isPlayingLowerPriority(AudioEventRTS *event) const override;
    virtual Bool isPlayingAlready(AudioEventRTS *event) const override;
    virtual Bool isObjectPlayingVoice(UnsignedInt objID) const override;

    virtual void adjustVolumeOfPlayingAudio(AsciiString eventName, Real newVolume) override;
    virtual void removePlayingAudio(AsciiString eventName) override;
    virtual void removeAllDisabledAudio() override;

    virtual Bool has3DSensitiveStreamsPlaying(void) const override;

    virtual void *getHandleForBink(void) override;
    virtual void releaseHandleForBink(void) override;

    virtual void friend_forcePlayAudioEventRTS(const AudioEventRTS *eventToPlay) override;

    virtual void setPreferredProvider(AsciiString providerNdx) override;
    virtual void setPreferredSpeaker(AsciiString speakerType) override;

    virtual Real getFileLengthMS(AsciiString strToLoad) const override;

    virtual void closeAnySamplesUsingFile(const void *fileToClose) override;

    // --- Oboe callbacks ---
    virtual oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;
    virtual void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;

    // --- Request processing (mirrors Miles pattern) ---
    virtual void processRequestList(void) override;

protected:
    virtual void setDeviceListenerPosition(void) override;

private:
    void openStream();
    void closeStream();

    // Audio source management
    void playAudioEvent(AudioEventRTS *event);
    void stopAudioEvent(AudioHandle handle);
    int16_t *loadWavFile(const char *filename, uint32_t &outSampleCount, int &outChannels, int &outSampleRate);
    void cleanupFinishedAudio();

    // Oboe stream
    std::shared_ptr<oboe::AudioStream> m_stream;
    bool m_isStreamOpen;

    // Active audio sources (accessed from both game thread and audio callback)
    std::mutex m_audioMutex;
    std::vector<AndroidPlayingAudio*> m_playingSources;
    std::vector<AndroidPlayingAudio*> m_stoppedSources;

    // Intermediate mix buffer (32-bit to avoid clipping during mixing)
    std::vector<int32_t> m_mixBuffer;
};
