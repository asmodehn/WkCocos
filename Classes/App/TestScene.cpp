#include "WkCocosApp/TestScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/TimerUI.h"

#include "WkCocosApp/ErrorUI.h"

#include <iostream>
#include <numeric>

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

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(TestScene::menuCloseCallback, this));
	closeItem->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto menu = cocos2d::Menu::create(closeItem, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	addChild(menu, 1);

	//Saving UI
	SavingUI* saveui = new SavingUI();
	auto saveroot = saveui->getRoot();
	saveroot->setEnabled(true);
	saveroot->setVisible(true);
	addChild(saveroot);
	m_ui[SavingUI::id] = saveui;
	saveroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.25, visibleSize.height * 0.75));
	//*/

	//TimerUI
	TimerUI* timerui = new TimerUI();
	auto timerroot = timerui->getRoot();
	timerroot->setEnabled(true);
	timerroot->setVisible(true);
	addChild(timerroot);
	m_ui[TimerUI::id] = timerui;
	timerroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.75, visibleSize.height * 0.75));
	//*/

	//Error UI
	ErrorUI* errorui = new ErrorUI();
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	errorroot->setEnabled(false);
	errorroot->setVisible(false);
	m_ui[ErrorUI::id] = errorui;
	errorroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));
	//*/

	errorui->setRefreshCallback([this, errorui](){
		
		errorui->deactivate();

	});

	errorui->setSkipCallback([this, errorui](){

		errorui->deactivate();

	});

	auto sprite = cocos2d::Sprite::create("HelloWorld.png");
	sprite->setScaleX((visibleSize.width / 2 - closeItem->getContentSize().width) / sprite->getContentSize().width);
	sprite->setScaleY((visibleSize.height / 2 - closeItem->getContentSize().height) / sprite->getContentSize().height);
	sprite->setPosition(cocos2d::Vec2((visibleSize.width / 2 + closeItem->getPositionX() - closeItem->getContentSize().width / 2) / 2,
		(visibleSize.height / 2 + closeItem->getPositionY() + closeItem->getContentSize().height / 2) / 2));
	addChild(sprite, 0);

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

void TestScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}