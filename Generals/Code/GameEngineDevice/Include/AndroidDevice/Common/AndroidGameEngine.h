#pragma once

#include "Common/GameEngine.h"

struct android_app;

class AndroidGameEngine : public GameEngine
{
public:

	AndroidGameEngine();
	virtual ~AndroidGameEngine();

	virtual void init( void );															///< initialization
	virtual void reset( void );															///< reset engine
	virtual void update( void );														///< update the game engine
	virtual void serviceWindowsOS( void );									///< allow windows maintenance in background
    
    static void setAndroidApp(struct android_app* app);
    static struct android_app* getAndroidApp();

protected:
    static struct android_app* m_androidApp;


	virtual GameLogic *createGameLogic( void );							///< factory for game logic
 	virtual GameClient *createGameClient( void );						///< factory for game client
	virtual ModuleFactory *createModuleFactory( void );			///< factory for creating modules
	virtual ThingFactory *createThingFactory( void );				///< factory for the thing factory
	virtual FunctionLexicon *createFunctionLexicon( void ); ///< factory for function lexicon
	virtual LocalFileSystem *createLocalFileSystem( void ); ///< factory for local file system
	virtual ArchiveFileSystem *createArchiveFileSystem( void );	///< factory for archive file system
	virtual NetworkInterface *createNetwork( void );				///< Factory for the network
	virtual Radar *createRadar( void );											///< Factory for radar
	virtual WebBrowser *createWebBrowser( void );						///< Factory for embedded browser
	virtual AudioManager *createAudioManager( void );				///< Factory for audio device
	virtual ParticleSystemManager* createParticleSystemManager( void );

};
