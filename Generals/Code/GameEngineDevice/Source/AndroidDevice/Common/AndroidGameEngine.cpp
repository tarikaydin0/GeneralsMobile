#include "AndroidDevice/Common/AndroidGameEngine.h"
#include "AndroidDevice/Common/AndroidUIScale.h"
#include "GameLogic/GameLogic.h"
#include "GameNetwork/NetworkInterface.h"
#include "AndroidDevice/Audio/AndroidAudioManager.h"
#include "StdDevice/Common/StdBIGFileSystem.h"
#include "StdDevice/Common/StdLocalFileSystem.h"
#include "W3DDevice/Common/W3DModuleFactory.h"
#include "W3DDevice/GameLogic/W3DGameLogic.h"
#include "W3DDevice/GameClient/W3DGameClient.h"
#include "W3DDevice/Common/W3DFunctionLexicon.h"
#include "W3DDevice/Common/W3DRadar.h"
#include "W3DDevice/Common/W3DThingFactory.h"
#include "W3DDevice/GameClient/W3DParticleSys.h"

// AndroidAudioManager uses Google Oboe for low-latency audio on Android.

#include <android/native_window.h>
#include <android/configuration.h>
#include <android/log.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#define LOG_TAG "AndroidGameEngine"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

struct android_app;
struct android_app* AndroidGameEngine::m_androidApp = nullptr;

void AndroidGameEngine::setAndroidApp(struct android_app* app) {
    m_androidApp = app;
}

struct android_app* AndroidGameEngine::getAndroidApp() {
    return m_androidApp;
}

AndroidGameEngine::AndroidGameEngine() {
}

AndroidGameEngine::~AndroidGameEngine() {
}

void AndroidGameEngine::init( void ) {
    // Query native display dimensions and DPI before engine init.
    if (m_androidApp && m_androidApp->window) {
        Int w = ANativeWindow_getWidth(m_androidApp->window);
        Int h = ANativeWindow_getHeight(m_androidApp->window);

        Int dpi = 160; // default mdpi
        if (m_androidApp->config) {
            dpi = AConfiguration_getDensity(m_androidApp->config);
            if (dpi <= 0) dpi = 160;
        }

        LOGI("Native display: %dx%d @ %d dpi", w, h, dpi);
        AndroidUIScale::init(w, h, dpi);

        // Override engine resolution to match device.
        if (TheWritableGlobalData) {
            TheWritableGlobalData->m_xResolution = w;
            TheWritableGlobalData->m_yResolution = h;
            TheWritableGlobalData->m_windowed = FALSE;
            LOGI("Resolution overridden to %dx%d", w, h);
        }
    }

    GameEngine::init();
}

void AndroidGameEngine::reset( void ) {
    GameEngine::reset();
}

void AndroidGameEngine::update( void ) {
    GameEngine::update();
}

void AndroidGameEngine::serviceWindowsOS( void ) {
    // Android main loop handles OS events in android_main.cpp
}

GameLogic *AndroidGameEngine::createGameLogic( void ) { return new W3DGameLogic; }
GameClient *AndroidGameEngine::createGameClient( void ) { return new W3DGameClient; }
ModuleFactory *AndroidGameEngine::createModuleFactory( void ) { return new W3DModuleFactory; }
ThingFactory *AndroidGameEngine::createThingFactory( void ) { return new W3DThingFactory; }
FunctionLexicon *AndroidGameEngine::createFunctionLexicon( void ) { return new W3DFunctionLexicon; }

LocalFileSystem *AndroidGameEngine::createLocalFileSystem( void ) { return new StdLocalFileSystem; }
ArchiveFileSystem *AndroidGameEngine::createArchiveFileSystem( void ) { return new StdBIGFileSystem; }

NetworkInterface *AndroidGameEngine::createNetwork( void ) { return NetworkInterface::createNetwork(); }
Radar *AndroidGameEngine::createRadar( void ) { return new W3DRadar; }
WebBrowser *AndroidGameEngine::createWebBrowser( void ) { return nullptr; } 
AudioManager *AndroidGameEngine::createAudioManager( void ) { return new AndroidAudioManager; }
ParticleSystemManager* AndroidGameEngine::createParticleSystemManager( void ) { return new W3DParticleSystemManager; }
