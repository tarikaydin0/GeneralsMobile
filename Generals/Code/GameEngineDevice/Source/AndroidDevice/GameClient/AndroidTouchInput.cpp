#include "AndroidDevice/GameClient/AndroidTouchInput.h"
#include <android/input.h>
#include <cmath>
#include <cstring> // memset

AndroidTouchInput::AndroidTouchInput() : Mouse() {
    m_curX = 0;
    m_curY = 0;
    m_isDown = false;
    memset(m_pointers, 0, sizeof(m_pointers));
    m_lastPinchDist = -1.0f;
}

AndroidTouchInput::~AndroidTouchInput() {
}

void AndroidTouchInput::init( void ) {
    Mouse::init();
}

void AndroidTouchInput::update( void ) {
    Mouse::update();
}

void AndroidTouchInput::reset( void ) {
    m_eventQueue.clear();
    m_isDown = false;
    memset(m_pointers, 0, sizeof(m_pointers)); // Reset pointers
    m_lastPinchDist = -1.0f;
}

void AndroidTouchInput::handleTouchEvent(int action, int pointerId, int x, int y) {
    if (pointerId < 0 || pointerId >= 10) return;
    TouchEvent e;
    e.action = action;
    e.pointerId = pointerId;
    e.x = x;
    e.y = y;
    m_eventQueue.push_back(e);
}

UnsignedByte AndroidTouchInput::getMouseEvent( MouseIO *result, Bool flush ) {
    if (flush) {
        m_eventQueue.clear();
        return MOUSE_NONE;
    }

    if (m_eventQueue.empty()) return MOUSE_NONE;

    TouchEvent e = m_eventQueue.front();
    m_eventQueue.erase(m_eventQueue.begin());

    // Update global pointer state
    int pid = e.pointerId;
    int maskedAction = e.action & AMOTION_EVENT_ACTION_MASK;

    if (maskedAction == AMOTION_EVENT_ACTION_DOWN || 
        maskedAction == AMOTION_EVENT_ACTION_POINTER_DOWN) {
        m_pointers[pid].active = true;
        m_pointers[pid].x = e.x;
        m_pointers[pid].y = e.y;
    } else if (maskedAction == AMOTION_EVENT_ACTION_UP || 
               maskedAction == AMOTION_EVENT_ACTION_POINTER_UP || 
               maskedAction == AMOTION_EVENT_ACTION_CANCEL) {
        m_pointers[pid].active = false;
    } else if (maskedAction == AMOTION_EVENT_ACTION_MOVE) {
        m_pointers[pid].x = e.x;
        m_pointers[pid].y = e.y;
    }

    // Default result fields
    result->time = 0; 
    result->wheelPos = 0;
    result->leftState = MBS_None;
    result->rightState = MBS_None;
    result->middleState = MBS_None;
    result->leftEvent = MOUSE_EVENT_NONE;
    result->rightEvent = MOUSE_EVENT_NONE;
    result->middleEvent = MOUSE_EVENT_NONE;

    // Pinch Zoom Logic (Pointers 0 and 1)
    if (m_pointers[0].active && m_pointers[1].active) {
        float dx = (float)(m_pointers[0].x - m_pointers[1].x);
        float dy = (float)(m_pointers[0].y - m_pointers[1].y);
        float dist = sqrtf(dx*dx + dy*dy);
        
        if (m_lastPinchDist > 0.0f) {
            float delta = dist - m_lastPinchDist;
            if (std::abs(delta) > 10.0f) { // Threshold
                 // Zoom
                 // Scaling: wheel delta 120 per click.
                 // Say 10 pixels = 120 units?
                 result->wheelPos = (int)(delta * 2.0f); // Tweak scale
                 m_lastPinchDist = dist;
            }
        } else {
            m_lastPinchDist = dist;
        }
        
        // Suppress mouse movement if pinching?
        // For now, let Pointer 0 drive mouse pos still.
    } else {
        m_lastPinchDist = -1.0f;
    }

    // Processing Pointer 0 for standard mouse emulation
    if (pid == 0) {
        result->pos.x = e.x;
        result->pos.y = e.y;
        result->deltaPos.x = e.x - m_curX; 
        result->deltaPos.y = e.y - m_curY;
        m_curX = e.x;
        m_curY = e.y;
        
        if (maskedAction == AMOTION_EVENT_ACTION_DOWN) {
            result->leftState = MBS_Down;
            m_isDown = true;
        } else if (maskedAction == AMOTION_EVENT_ACTION_UP) {
            result->leftState = MBS_Up;
            m_isDown = false;
        } else if (maskedAction == AMOTION_EVENT_ACTION_MOVE) {
            if (m_isDown) result->leftState = MBS_Down;
            else result->leftState = MBS_Up;
        }
    } else {
        // For other pointers, we just consume the event for state tracking
        // Keep previous position/state for result
        result->pos.x = m_curX;
        result->pos.y = m_curY;
        result->deltaPos.x = 0;
        result->deltaPos.y = 0;
        if (m_isDown) result->leftState = MBS_Down;
        else result->leftState = MBS_Up;
    }

    return MOUSE_OK;
}
