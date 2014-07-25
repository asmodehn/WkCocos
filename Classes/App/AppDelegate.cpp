#include "WkCocosApp/AppDelegate.h"
#include "WkCocosApp/LoadingScene.h"
#include "WkCocosApp/HelloWorldScene.h"

#include "WkCocos/WkCocos.h"

USING_NS_CC;

AppDelegate::AppDelegate()
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

	WkCocos::LogStream logger;
	logger << "Awesone log";

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {

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
    LoadingScene* loadscene = LoadingScene::create();

	loadscene->scheduleDLCCheck();

	loadscene->setLoadDoneCallback([](){
		auto director = cocos2d::Director::getInstance();
		director->replaceScene(cocos2d::TransitionFade::create(1.0f, HelloWorld::createScene()));
	});

	//Initializing App42
	WkCocos::App42::Setup("3a3579d378cdf38a29e7dd80ec20dc15fc2a19a6959bcfc1ea353885a1802f86", "89ff08c30c0f3d15e5b571d2b3a90fd80401a756cb7f3620cfc625756421ee35");
	WkCocos::App42::Login();

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
}
