#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <android/log.h>
#include <unistd.h>
#include <windows.h> // Mock windows types

#include "AndroidDevice/Common/AndroidGameEngine.h"
#include "Common/GameEngine.h"
#include <android/input.h>
#include "GameClient/GameClient.h"
#include "GameClient/Mouse.h"
#include "AndroidDevice/GameClient/AndroidTouchInput.h"

#define LOG_TAG "GeneralsMobile"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global variables expected by the engine
HWND ApplicationHWnd = nullptr;
HINSTANCE ApplicationHInstance = nullptr;

static bool g_initialized = false;
static bool g_hasWindow = false;

// Handle Input Events
static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int action = AMotionEvent_getAction(event);
        int maskedAction = action & AMOTION_EVENT_ACTION_MASK;

        if (TheMouse) {
            AndroidTouchInput* touchInput = static_cast<AndroidTouchInput*>(TheMouse);
            
            if (maskedAction == AMOTION_EVENT_ACTION_MOVE) {
                size_t count = AMotionEvent_getPointerCount(event);
                for (size_t i = 0; i < count; i++) {
                    int ptrId = AMotionEvent_getPointerId(event, i);
                    touchInput->handleTouchEvent(maskedAction, 
                                                 ptrId,
                                                 (int)AMotionEvent_getX(event, i), 
                                                 (int)AMotionEvent_getY(event, i));
                }
            } else {
                int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                int ptrId = AMotionEvent_getPointerId(event, index);
                touchInput->handleTouchEvent(maskedAction, 
                                             ptrId, // Pass pointer ID
                                             (int)AMotionEvent_getX(event, index), 
                                             (int)AMotionEvent_getY(event, index));
            }
            return 1;
        }
    }
    return 0;
}

// Handle Android lifecycle commands
static void handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != nullptr) {
                LOGI("APP_CMD_INIT_WINDOW: Window created");
                ApplicationHWnd = (HWND)app->window;
                g_hasWindow = true;

                if (!g_initialized) {
                    LOGI("Initializing Game Engine");
                    if (!TheGameEngine) {
                        TheGameEngine = new AndroidGameEngine();
                    }
                    TheGameEngine->init();
                    g_initialized = true;
                    LOGI("Game Engine Initialized");
                } else {
                    // Re-attaching window?
                    // Might need to update wrapper's window reference if it changed
                    // But init() usually handles passing the HWND.
                    // If strictly re-init is needed, we might need a distinct path.
                    // For now, assume window surface recreation is handled by EGL/Wrapper inside update or internal checks.
                    
                    // Actually, DX8Wrapper::Init uses the HWND to create EGL Surface.
                    // We might need to call something to update the window.
                    // TODO: Handle resume/window recreation properly in DX8Wrapper
                }
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW: Window destroyed");
            g_hasWindow = false;
            ApplicationHWnd = nullptr;
            break;
        case APP_CMD_GAINED_FOCUS:
            if (TheGameEngine) TheGameEngine->setIsActive(TRUE);
            break;
        case APP_CMD_LOST_FOCUS:
            if (TheGameEngine) TheGameEngine->setIsActive(FALSE);
            break;
    }
}

// Main entry point
extern "C" {
    void android_main(struct android_app* state) {
        LOGI("Entering android_main");
        
        AndroidGameEngine::setAndroidApp(state);
        state->onAppCmd = handle_cmd;
        state->onInputEvent = handle_input;

        // Change working directory to external storage for game data
        // TODO: This path should be dynamic or properly requested
        const char* dataPath = "/storage/emulated/0/Android/data/com.generals.mobile/files";
        if (chdir(dataPath) == 0) {
            LOGI("Changed working directory to: %s", dataPath);
        } else {
            LOGE("Failed to change working directory to: %s. Game data might not be found.", dataPath);
        }

        while (true) {
            int events;
            struct android_poll_source* source;
            
            // If initialized and has window, don't block (animate). Otherwise block.
            int timeout = (g_initialized && g_hasWindow) ? 0 : -1;
            
            while ((ALooper_pollOnce(timeout, nullptr, &events, (void**)&source)) >= 0) {
                if (source != nullptr) {
                    source->process(state, source);
                }
                
                if (state->destroyRequested != 0) {
                    LOGI("Destroy requested, exiting loop");
                    // Can perform cleanup here
                    return;
                }
            }

            // Game Loop
            if (g_initialized && g_hasWindow && TheGameEngine) {
                TheGameEngine->update();
            }
        }
    }
}
