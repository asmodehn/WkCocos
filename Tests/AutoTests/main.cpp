// autotest.cpp
#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <cstring>

//using a minimum appdelegate to have a cocos update loop running ( needed for all entity systems to work )

#include "WkCocos/WkCocos.h"
#include "WkCocos/Utils/jni/Utils.h"

int mem_argc;
std::vector<std::string> mem_argv;

/**
@brief    The cocos2d Application.
*/
class  AppDelegate : private cocos2d::Application, public entityx::Receiver<AppDelegate>
{
public:
	AppDelegate()
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
#endif

		WkCocos::LogStream::create();

		m_consoleApp = new WkCocos::CLogAppender(); //doesnt print in device console.
		m_fileApp = new WkCocos::FileLogAppender(cocos2d::FileUtils::getInstance()->getWritablePath() + "my.log");

		m_consoleApp->setLevel(WkCocos::loglevel::Core_LogInfo);
		m_fileApp->setLevel(WkCocos::loglevel::Core_LogDebug);

		WkCocos::LogStream::get()->addAppender(m_consoleApp);
		WkCocos::LogStream::get()->addAppender(m_fileApp);

		LOG_INFO << "This is an INFO level log" << std::endl;
		LOG_DEBUG << "This is a DEBUG level log" << std::endl;
	}

	virtual ~AppDelegate()
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

	virtual void initGLContextAttrs()
	{
		//set OpenGL context attributions,now can only set six attributions:
		//red,green,blue,alpha,depth,stencil
		GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
		cocos2d::GLView::setGLContextAttrs(glContextAttrs);
	}

	/**
	@brief    Implement Director and Scene init code here.
	@return true    Initialize success, app continue.
	@return false   Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching()
	{
		// initialize director
		auto director = cocos2d::Director::getInstance();
		auto glview = director->getOpenGLView();
		if (!glview) {
			glview = cocos2d::GLViewImpl::create("My Game");
			director->setOpenGLView(glview);
		}

		// turn on display FPS
		//director->setDisplayStats(true);

		// set FPS. the default value is 1.0/60 if you don't call this
		//director->setAnimationInterval(1.0 / 60);

		// create a scene. it's an autorelease object
		testscene = cocos2d::Scene::create();

		// run
		director->runWithScene(cocos2d::TransitionFade::create(1.0f, testscene));
		
		if (mem_argc > 1)
		{
			//if first arg is "suite", we search for suite names instead of test names
			const bool bSuite = "suite" == mem_argv[1];

			if (bSuite)
			{ //running test suite
				std::vector<char const * > suites;
				for (int i = 2; i < mem_argv.size(); ++i)
				{
					suites.push_back(mem_argv[i].c_str());
				}
				return UnitTest::RunAllTestsInSuites(suites);
			}
			else
			{ // running tests
				std::vector<char const * > tests;
				for (int i = 1; i < mem_argv.size(); ++i)
				{
					tests.push_back(mem_argv[i].c_str());
				}
				return UnitTest::RunAllNamedTests(tests);
			}
		}
		else
		{
			return UnitTest::RunAllTests();
		}
	}

	/**
	@brief  The function be called when the application enter background
	@param  the pointer of the application
	*/
	virtual void applicationDidEnterBackground()
	{
		cocos2d::Director::getInstance()->stopAnimation();

		// if you use SimpleAudioEngine, it must be pause
		// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	/**
	@brief  The function be called when the application enter foreground
	@param  the pointer of the application
	*/
	virtual void applicationWillEnterForeground()
	{

		cocos2d::Director::getInstance()->startAnimation();

		// if you use SimpleAudioEngine, it must resume here
		// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

		std::string vstring = WkCocos::Utils::jni::Utils::getVersionName();
		CCLOG("Version string from app : %s", vstring.c_str());
	}
	
private:

	/**
	* Basic Test Scene
	*/
	cocos2d::Scene* testscene;

	/**
	* Log appender: Console
	*/
	WkCocos::CLogAppender*		m_consoleApp;

	/**
	* Log appender: File
	*/
	WkCocos::FileLogAppender*	m_fileApp;

	/**
	* Log appender: Cocos
	*/
	WkCocos::CocosLogAppender*	m_cocosApp;
};

int main(int argc, char** argv)
{
	//grabbing args
	mem_argc = argc;
	for (int i = 1; i < mem_argc; ++i)
	{
		mem_argv.push_back(argv[i]);
	}

	// create the application instance
	AppDelegate app;
	return cocos2d::Application::getInstance()->run();

	
}
