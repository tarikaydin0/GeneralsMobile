/*
**  AndroidVirtualControls.h
**  On-screen virtual buttons for essential RTS keyboard shortcuts.
**
**  Provides touch-friendly access to commonly used hotkeys like
**  Attack Move (A), Stop (S), Deploy (D), Sell ($), and Scatter (X).
*/

#pragma once

#include "Lib/BaseType.h"

struct VirtualButton
{
    Int x, y, width, height;   // Screen position & size (pixels)
    Int keyCode;               // Win32 virtual key code to inject
    const char* label;         // Display label
    Bool pressed;
};

class AndroidVirtualControls
{
public:
    static void init(Int screenWidth, Int screenHeight, Int minTouchPx);

    /// Process a touch event; returns true if consumed by a virtual button.
    static Bool handleTouch(Int action, Int touchX, Int touchY);

    /// Get buttons for drawing (engine calls this during UI overlay pass).
    static const VirtualButton* getButtons(Int& outCount);

    /// Toggle visibility.
    static void setVisible(Bool visible) { s_visible = visible; }
    static Bool isVisible() { return s_visible; }

private:
    static void layoutButtons(Int screenWidth, Int screenHeight, Int btnSize, Int margin);

    enum { MAX_BUTTONS = 8 };
    static VirtualButton s_buttons[MAX_BUTTONS];
    static Int  s_buttonCount;
    static Bool s_visible;
};
