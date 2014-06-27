#include "WkCocosApp/AppDelegate.h"
#include "WkCocosApp/HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
	//initializing search paths for different platforms
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	cocos2d::FileUtils::getInstance()->addSearchPath("Resources");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
	cocos2d::FileUtils::getInstance()->addSearchPath("Resources");
	//NOT NEEDED. COCOS DOES IT ALREADY.
	//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//		cocos2d::FileUtils::getInstance()->addSearchPath("assets");
#endif


}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {

	//TMP testing ToolBox
	std::string fortytwo = WkCocos::ToolBox::itoa(42);
	CCLOG("fortytwo : %s", fortytwo.c_str());

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

	//Initializing App42
	WkCocos::App42::Setup("eb075343180ff254993d760eaeff219d9c6f3cd768c976f4cb7c25e6a6a7a88e", "e3a38692cb56a84998eaa0b74db029fcfcd331b56c7d82d22a3997e7ca0a85cb");
	WkCocos::App42::Login();

    // run
    director->runWithScene(scene);

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
}
