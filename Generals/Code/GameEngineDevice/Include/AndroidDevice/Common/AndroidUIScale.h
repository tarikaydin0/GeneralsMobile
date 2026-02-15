/*
**  AndroidUIScale.h
**  DPI-aware scale factor for adapting 800x600 game UI to Android screens.
*/

#pragma once

#include "Lib/BaseType.h"

class AndroidUIScale
{
public:
    static void init(Int nativeWidth, Int nativeHeight, Int dpi);

    /// Scale factor relative to the game's base resolution (800x600).
    static Real getScaleFactor() { return s_scaleFactor; }

    /// Convenience: scale a coordinate value.
    static Int scaleX(Int x) { return (Int)(x * s_scaleX); }
    static Int scaleY(Int y) { return (Int)(y * s_scaleY); }

    /// Native device dimensions.
    static Int getNativeWidth()  { return s_nativeWidth; }
    static Int getNativeHeight() { return s_nativeHeight; }
    static Int getDPI()          { return s_dpi; }

    /// Minimum touch target size in pixels (48dp).
    static Int getMinTouchTarget() { return s_minTouchTarget; }

private:
    static Real s_scaleFactor;
    static Real s_scaleX;
    static Real s_scaleY;
    static Int  s_nativeWidth;
    static Int  s_nativeHeight;
    static Int  s_dpi;
    static Int  s_minTouchTarget;

    // Base resolution the game was designed for.
    static constexpr Int BASE_WIDTH  = 800;
    static constexpr Int BASE_HEIGHT = 600;
    // Material Design minimum touch target.
    static constexpr Int MIN_TOUCH_DP = 48;
};
