#include "WkCocosApp/SavingScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/ShopUI.h"

#include "WkCocosApp/ErrorUI.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <numeric>

USING_NS_CC;

SavingScene::SavingScene() : Scene()
{
}

SavingScene::~SavingScene()
{
}

// on "init" you need to initialize your instance
bool SavingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	//cocos2d::Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	//Load UI
	//SavingUI* saveui = new SavingUI();
	//saveui->getRoot()->setEnabled(true);
	//saveui->getRoot()->setVisible(true);
	//addChild(saveui->getRoot());
	//m_ui[SavingUI::id] = saveui;

	//ShopUI
	ShopUI* shopui = new ShopUI();
	shopui->getRoot()->setEnabled(true);
	shopui->getRoot()->setVisible(true);
	addChild(shopui->getRoot());
	m_ui[ShopUI::id] = shopui;

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

void SavingScene::onEnterTransitionDidFinish()
{
	//launching update method
	scheduleUpdate();
}

void SavingScene::onExitTransitionDidStart()
{

}


void SavingScene::update(float delta)
{
	Scene::update(delta);
}

void SavingScene::error_CB()
{
	CCLOGERROR("ERROR");

	//Error UI
	ErrorUI* errorui = getInterface<ErrorUI>(ErrorUI::id);
	errorui->activate();
	
}