#pragma once
#include "GameClient/Mouse.h"
#include <vector>

class AndroidTouchInput : public Mouse
{
public:
    AndroidTouchInput();
    virtual ~AndroidTouchInput();

    virtual void init( void );
    virtual void update( void );
    virtual void reset( void );
    virtual void setCursor( MouseCursor cursor ) {}
    virtual void showCursor( Bool show ) {}
    virtual void setCursorPos( const ICoord2D *pos ) {}
    
    // Custom method for Android input injection
    void handleTouchEvent(int action, int pointerId, int x, int y);

protected:
    virtual void capture( void ) {}
    virtual void releaseCapture( void ) {}
    virtual UnsignedByte getMouseEvent( MouseIO *result, Bool flush );

private:
    struct TouchEvent {
        int action; // AMOTION_EVENT_ACTION_*
        int pointerId;
        int x;
        int y;
    };
    std::vector<TouchEvent> m_eventQueue;
    
    // Multi-touch state
    struct PointerState {
        int x;
        int y;
        bool active;
    };
    PointerState m_pointers[10]; // Max 10 pointers
    float m_lastPinchDist = -1.0f;

    // Helper to store current state for immediate queries if needed
    int m_curX = 0;
    int m_curY = 0;
    bool m_isDown = false;
};
