#include "WkCocosApp/TestScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/TimerUI.h"

#include "WkCocosApp/ErrorUI.h"

#include <iostream>
#include <numeric>

USING_NS_CC;

TestScene::TestScene() : Scene()
{
}

TestScene::~TestScene()
{
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	//cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Saving UI
	SavingUI* saveui = new SavingUI();
	auto saveroot = saveui->getRoot();
	saveroot->setEnabled(true);
	saveroot->setVisible(true);
	addChild(saveroot);
	m_ui[SavingUI::id] = saveui;
	saveroot->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.75));
	//*/

	//TimerUI
	TimerUI* timerui = new TimerUI();
	auto timerroot = timerui->getRoot();
	timerroot->setEnabled(true);
	timerroot->setVisible(true);
	addChild(timerroot);
	m_ui[TimerUI::id] = timerui;
	timerroot->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.75));
	//*/

	//Error UI
	ErrorUI* errorui = new ErrorUI();
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	errorroot->setEnabled(false);
	errorroot->setVisible(false);
	m_ui[ErrorUI::id] = errorui;
	errorroot->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));
	//*/

	errorui->setRefreshCallback([this, errorui](){
		
		errorui->deactivate();

	});

	errorui->setSkipCallback([this, errorui](){

		errorui->deactivate();

	});

	return true;
}

void TestScene::onEnterTransitionDidFinish()
{
	//launching update method
	scheduleUpdate();
}

void TestScene::onExitTransitionDidStart()
{

}

void TestScene::update(float delta)
{
	Scene::update(delta);
}

void TestScene::error_CB()
{
	CCLOGERROR("ERROR");

	//Error UI
	ErrorUI* errorui = getInterface<ErrorUI>(ErrorUI::id);
	errorui->activate();
	
}