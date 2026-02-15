/*
**  AndroidVideoPlayer.h
**  Video player stub for Android. Skips Bink videos gracefully.
**  Future: integrate FFmpeg for .bik / .mp4 decoding.
*/

#pragma once

#include "GameClient/VideoPlayer.h"

// ---------------------------------------------------------------------------
// AndroidVideoStream — a video stream that reports zero frames (skip video).
// ---------------------------------------------------------------------------
class AndroidVideoStream : public VideoStream
{
    friend class AndroidVideoPlayer;

public:
    virtual ~AndroidVideoStream() override;

    virtual void update(void) override {}
    virtual void close(void) override;

    virtual Bool isFrameReady(void) override { return FALSE; }
    virtual void frameDecompress(void) override {}
    virtual void frameRender(VideoBuffer *buffer) override {}
    virtual void frameNext(void) override {}
    virtual Int  frameIndex(void) override { return 0; }
    virtual Int  frameCount(void) override { return 0; }
    virtual void frameGoto(Int index) override {}
    virtual Int  height(void) override { return 0; }
    virtual Int  width(void) override { return 0; }

private:
    AndroidVideoStream() {}
};

// ---------------------------------------------------------------------------
// AndroidVideoPlayer — video player that returns empty streams (skip video).
// ---------------------------------------------------------------------------
class AndroidVideoPlayer : public VideoPlayer
{
public:
    AndroidVideoPlayer();
    virtual ~AndroidVideoPlayer();

    virtual VideoStreamInterface* open(AsciiString movieTitle) override;
    virtual VideoStreamInterface* load(AsciiString movieTitle) override;
};
