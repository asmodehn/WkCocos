#include "WkCocosApp/AppDelegate.h"
#include "WkCocosApp/LoadingScene.h"
#include "WkCocosApp/TestScene.h"
#include "WkCocosApp/GameLogic.h"

#include "WkCocos/Utils/jni/Utils.h"

USING_NS_CC;

AppDelegate::AppDelegate()
: m_consoleApp(nullptr)
, m_fileApp(nullptr)
, m_cocosApp(nullptr)
{
	//initializing search paths for different platforms
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	cocos2d::FileUtils::getInstance()->addSearchPath("Resources");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
	//somehow on linux the folder is not the folder where the app is run from (build/)
	// but the folder where the exe is located (build/Tests).
	cocos2d::FileUtils::getInstance()->addSearchPath("../Resources");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//NOT NEEDED. COCOS DOES IT ALREADY.
	//cocos2d::FileUtils::getInstance()->addSearchPath("assets");
#endif

	WkCocos::LogStream::create();

	m_consoleApp = new WkCocos::CLogAppender(); //doesnt print in device console.
	m_fileApp = new WkCocos::FileLogAppender(FileUtils::getInstance()->getWritablePath() + "my.log");

	m_consoleApp->setLevel(WkCocos::loglevel::Core_LogInfo);
	m_fileApp->setLevel(WkCocos::loglevel::Core_LogDebug);

	WkCocos::LogStream::get()->addAppender(m_consoleApp);
	WkCocos::LogStream::get()->addAppender(m_fileApp);

	LOG_INFO << "This is an INFO level log" << std::endl;
	LOG_DEBUG << "This is a DEBUG level log" << std::endl;
}

AppDelegate::~AppDelegate()
{
	if (m_fileApp)
	{
		WkCocos::LogStream::get()->removeAppender(m_fileApp);
		delete m_fileApp;
		m_fileApp = nullptr;
	}
	if (m_consoleApp)
	{
		WkCocos::LogStream::get()->removeAppender(m_consoleApp);
		delete m_consoleApp;
		m_consoleApp = nullptr;
	}
	if (m_cocosApp)
	{
		WkCocos::LogStream::get()->removeAppender(m_cocosApp);
		delete m_cocosApp;
		m_cocosApp = nullptr;
	}
	WkCocos::LogStream::destroy();
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	LoadingScene* loadscene = LoadingScene::create();

	// Test for cocos log appender
	//WkCocos::CocosLogAppender::CheckBoxRes resource;
	//m_cocosApp = new WkCocos::CocosLogAppender(loadscene, "fonts/Text.fnt", resource);
	//WkCocos::LogStream::get()->addAppender(m_cocosApp);
	//LOG_INFO << "Add " << "cocos " << "appender" << std::endl;
	//LOG_INFO << "Next Line" << std::endl;

	//Creating gamelogic and setting player.
	g_gameLogic.reset( new GameLogic("73a0a556fbecbb4a8dd28728a06d7796f207d017cb6b74e8c9e45973ad487c14", "f7976c94667424a528a4723eb3e4791c24ecf9b36ec770c251b9e039faa04517", [loadscene]()
	{
		//We launch loading scene with DLC only after login
		loadscene->scheduleDLCCheck();

		loadscene->addLoad({ "HelloWorld.png" });

		loadscene->setLoadDoneCallback([](){
			auto director = cocos2d::Director::getInstance();
			director->replaceScene(cocos2d::TransitionFade::create(1.0f, TestScene::create()));
		});

	}));

	//TODO : improve the flow with a splash screen while login happens

    // run
	director->runWithScene(cocos2d::TransitionFade::create(1.0f, loadscene));

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

	std::string vstring = WkCocos::Utils::jni::Utils::getVersionName();
	CCLOG("Version string from app : %s",vstring.c_str());
}
