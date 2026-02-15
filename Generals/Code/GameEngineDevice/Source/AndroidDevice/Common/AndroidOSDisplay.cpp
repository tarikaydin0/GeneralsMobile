#include "Common/OSDisplay.h"
#include <android/log.h>

#define LOG_TAG "GeneralsMobile"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

OSDisplayButtonType OSDisplayWarningBox(AsciiString p, AsciiString m, UnsignedInt buttonFlags, UnsignedInt otherFlags)
{
    // TODO: Call JNI to show AlertDialog
    LOGW("OSDisplayWarningBox: %s - %s", p.str(), m.str());
    return OSDBT_OK;
}

void OSDisplaySetBusyState(Bool busyDisplay, Bool busySystem)
{
    // TODO: Keep screen on
    LOGI("OSDisplaySetBusyState: Display=%d, System=%d", busyDisplay, busySystem);
}
