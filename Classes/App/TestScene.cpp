#include "WkCocosApp/TestScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/TimerUI.h"

#include "WkCocosApp/ErrorUI.h"

#include "ui/CocosGUI.h"

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

	//cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	//cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	//Load UI
	//SavingUI* saveui = new SavingUI();
	//saveui->getRoot()->setEnabled(false);
	//saveui->getRoot()->setVisible(false);
	//addChild(saveui->getRoot());
	//m_ui[SavingUI::id] = saveui;

	//TimerUI
	TimerUI* timerui = new TimerUI();
	timerui->getRoot()->setEnabled(true);
	timerui->getRoot()->setVisible(true);
	addChild(timerui->getRoot());
	m_ui[TimerUI::id] = timerui;

	//Error UI
	ErrorUI* errorui = new ErrorUI();
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	errorroot->setEnabled(false);
	errorroot->setVisible(false);
	m_ui[ErrorUI::id] = errorui;

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