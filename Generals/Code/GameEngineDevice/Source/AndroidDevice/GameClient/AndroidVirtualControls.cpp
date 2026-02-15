/*
**  AndroidVirtualControls.cpp
**  On-screen virtual buttons for essential RTS keyboard shortcuts.
*/

#include "PreRTS.h"

#include "AndroidDevice/GameClient/AndroidVirtualControls.h"
#include "AndroidDevice/Common/AndroidUIScale.h"

#include <android/log.h>
#include <cstring>

#define LOG_TAG "AndroidVirtualControls"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Win32 virtual key codes (from our mock windows.h)
#ifndef VK_KEY_A
#define VK_KEY_A 0x41
#define VK_KEY_S 0x53
#define VK_KEY_D 0x44
#define VK_KEY_X 0x58
#define VK_ESCAPE 0x1B
#define VK_DELETE 0x2E
#endif

// Touch action constants (AMOTION_EVENT_ACTION_*)
#define ACTION_DOWN  0
#define ACTION_UP    1
#define ACTION_MOVE  2

// Static member initialisation
VirtualButton AndroidVirtualControls::s_buttons[MAX_BUTTONS];
Int  AndroidVirtualControls::s_buttonCount = 0;
Bool AndroidVirtualControls::s_visible = TRUE;

void AndroidVirtualControls::init(Int screenWidth, Int screenHeight, Int minTouchPx)
{
    Int btnSize = minTouchPx;
    if (btnSize < 48) btnSize = 48;
    Int margin = btnSize / 4;

    layoutButtons(screenWidth, screenHeight, btnSize, margin);
    LOGI("VirtualControls init: %d buttons, btnSize=%dpx", s_buttonCount, btnSize);
}

void AndroidVirtualControls::layoutButtons(Int screenWidth, Int screenHeight, Int btnSize, Int margin)
{
    s_buttonCount = 0;

    // Place buttons in a row at bottom-right corner, above the control bar area.
    // Layout:  [A] [S] [X] [Del] [Esc]
    struct ButtonDef { Int key; const char* label; };
    ButtonDef defs[] = {
        { VK_KEY_A, "ATK" },   // Attack Move
        { VK_KEY_S, "STP" },   // Stop
        { VK_KEY_X, "SCT" },   // Scatter
        { VK_DELETE, "DEL" },  // Delete unit
        { VK_ESCAPE, "ESC" },  // Escape / cancel
    };

    Int count = sizeof(defs) / sizeof(defs[0]);
    if (count > MAX_BUTTONS) count = MAX_BUTTONS;

    Int startX = screenWidth - (count * (btnSize + margin)) - margin;
    Int startY = screenHeight - btnSize - margin * 4; // above bottom edge

    for (Int i = 0; i < count; ++i) {
        VirtualButton& btn = s_buttons[i];
        btn.x       = startX + i * (btnSize + margin);
        btn.y       = startY;
        btn.width   = btnSize;
        btn.height  = btnSize;
        btn.keyCode = defs[i].key;
        btn.label   = defs[i].label;
        btn.pressed = FALSE;
    }
    s_buttonCount = count;
}

Bool AndroidVirtualControls::handleTouch(Int action, Int touchX, Int touchY)
{
    if (!s_visible) return FALSE;

    for (Int i = 0; i < s_buttonCount; ++i) {
        VirtualButton& btn = s_buttons[i];
        Bool inside = (touchX >= btn.x && touchX <= btn.x + btn.width &&
                       touchY >= btn.y && touchY <= btn.y + btn.height);

        if (inside) {
            if (action == ACTION_DOWN) {
                btn.pressed = TRUE;
                // TODO: Inject WM_KEYDOWN via AndroidTouchInput or direct PostMessage
                LOGI("VirtualButton '%s' pressed (key=0x%X)", btn.label, btn.keyCode);
                return TRUE;
            } else if (action == ACTION_UP) {
                btn.pressed = FALSE;
                // TODO: Inject WM_KEYUP
                LOGI("VirtualButton '%s' released (key=0x%X)", btn.label, btn.keyCode);
                return TRUE;
            }
        } else if (action == ACTION_UP) {
            btn.pressed = FALSE;
        }
    }
    return FALSE;
}

const VirtualButton* AndroidVirtualControls::getButtons(Int& outCount)
{
    outCount = s_buttonCount;
    return s_buttons;
}
