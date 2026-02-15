/*
**  AndroidVideoPlayer.cpp
**  Video player stub for Android.
**
**  Returns empty "zero-frame" streams so the engine gracefully
**  skips Bink cutscenes rather than crashing due to missing Bink DLL.
**  
**  Future: integrate FFmpeg or MediaCodec for actual .bik / .mp4 decoding.
*/

#include "PreRTS.h"

#include "AndroidDevice/GameClient/AndroidVideoPlayer.h"

#include <android/log.h>

#define LOG_TAG "AndroidVideoPlayer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// ---------------------------------------------------------------------------
// AndroidVideoStream
// ---------------------------------------------------------------------------

AndroidVideoStream::~AndroidVideoStream()
{
}

void AndroidVideoStream::close(void)
{
    if (m_player) {
        static_cast<AndroidVideoPlayer*>(m_player)->remove(this);
    }
}

// ---------------------------------------------------------------------------
// AndroidVideoPlayer
// ---------------------------------------------------------------------------

AndroidVideoPlayer::AndroidVideoPlayer()
{
    LOGI("AndroidVideoPlayer created (stub — videos will be skipped)");
}

AndroidVideoPlayer::~AndroidVideoPlayer()
{
}

VideoStreamInterface* AndroidVideoPlayer::open(AsciiString movieTitle)
{
    LOGI("open('%s') — skipping (no FFmpeg backend yet)", movieTitle.str());

    // Return nullptr so the engine knows no video is playing.
    // The callers (Display::playMovie, InGameUI::playMovie, LoadScreen)
    // already check for nullptr return.
    return nullptr;
}

VideoStreamInterface* AndroidVideoPlayer::load(AsciiString movieTitle)
{
    LOGI("load('%s') — skipping (no FFmpeg backend yet)", movieTitle.str());
    return nullptr;
}
