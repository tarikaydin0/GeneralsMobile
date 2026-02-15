/*
**  AndroidUIScale.cpp
**  DPI-aware scale factor for adapting 800x600 game UI to Android screens.
*/

#include "PreRTS.h"

#include "AndroidDevice/Common/AndroidUIScale.h"

#include <android/log.h>
#include <algorithm>

#define LOG_TAG "AndroidUIScale"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Static member initialisation
Real AndroidUIScale::s_scaleFactor  = 1.0f;
Real AndroidUIScale::s_scaleX       = 1.0f;
Real AndroidUIScale::s_scaleY       = 1.0f;
Int  AndroidUIScale::s_nativeWidth  = BASE_WIDTH;
Int  AndroidUIScale::s_nativeHeight = BASE_HEIGHT;
Int  AndroidUIScale::s_dpi          = 160;
Int  AndroidUIScale::s_minTouchTarget = MIN_TOUCH_DP;

void AndroidUIScale::init(Int nativeWidth, Int nativeHeight, Int dpi)
{
    s_nativeWidth  = nativeWidth;
    s_nativeHeight = nativeHeight;
    s_dpi          = (dpi > 0) ? dpi : 160;

    s_scaleX = (Real)nativeWidth  / (Real)BASE_WIDTH;
    s_scaleY = (Real)nativeHeight / (Real)BASE_HEIGHT;
    s_scaleFactor = std::min(s_scaleX, s_scaleY);

    // 48dp min touch target, converted to pixels.
    s_minTouchTarget = (Int)((Real)MIN_TOUCH_DP * ((Real)s_dpi / 160.0f));

    LOGI("UIScale init: native=%dx%d dpi=%d scaleX=%.2f scaleY=%.2f factor=%.2f minTouch=%dpx",
         nativeWidth, nativeHeight, dpi,
         s_scaleX, s_scaleY, s_scaleFactor,
         s_minTouchTarget);
}
