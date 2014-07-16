#include "WkCocosApp/LoadingScene.h"

#include "WkCocosApp/LoadingUI.h"

#include "WkCocosApp/ErrorUI.h"

//#include "WkCocosApp/HelloWorldScene.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <numeric>

USING_NS_CC;

LoadingScene::LoadingScene() : Scene()
, m_loadDoneCB_called(false)
, m_loadMan_del_scheduled(false)
, m_loadDoneCB()
, m_loadingManager(nullptr)
{
	LoadingScene::m_loadingManager = new WkCocos::Loading::LoadingManager(5, 1,
		std::bind(&LoadingScene::progress_CB, this, std::placeholders::_1),
		std::bind(&LoadingScene::error_CB, this));
}

LoadingScene::~LoadingScene()
{
	if (m_loadingManager)
	delete m_loadingManager;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
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
	LoadingUI* loadui = new LoadingUI();
	loadui->getRoot()->setEnabled(true);
	loadui->getRoot()->setVisible(true);
	addChild(loadui->getRoot());
	m_ui[LoadingUI::id] = loadui;

	//Error UI
	ErrorUI* errorui = new ErrorUI(this);
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	errorroot->setEnabled(false);
	errorroot->setVisible(false);
	m_ui[ErrorUI::id] = errorui;

	errorui->setRefreshCallback([](){
		/*
		auto ls = Director::getInstance()->getRunningScene();
		ls->m_loadingManager(5, 1,
			std::bind(&LoadingScene::progress_CB, ls, std::placeholders::_1),
			std::bind(&LoadingScene::error_CB, ls));
		*/

		/*
		ErrorUI* errorui = getInterface<ErrorUI>(ErrorUI::id);
		errorui->parent->m_loadingManager(5, 1,
			std::bind(&LoadingScene::progress_CB, errorui->parent, std::placeholders::_1),
			std::bind(&LoadingScene::error_CB, errorui->parent));
		*/
	});

	m_loadingManager->start();

	return true;
}

void LoadingScene::onEnterTransitionDidFinish()
{
	//launching update method
	scheduleUpdate();
}

void LoadingScene::onExitTransitionDidStart()
{

}

void LoadingScene::addLoad(std::vector<std::string> respath)
{
	if (m_loadingManager)
		m_loadingManager->addDataLoad(respath);
}

void LoadingScene::setLoadDoneCallback(std::function<void()> cb)
{
	m_loadDoneCB = cb;
}

void LoadingScene::scheduleDLCCheck()
{
	if (m_loadingManager)
		m_loadingManager->addDataDownload("manifest.json");
}

void LoadingScene::update(float delta)
{
	if (m_loadMan_del_scheduled)
	{
		delete m_loadingManager;
		m_loadingManager = nullptr;
		m_loadMan_del_scheduled = false;
	}

	//if callback was called, loading is finished.
	if (!m_loadDoneCB_called && m_loadingManager)
			m_loadingManager->step(delta);
}

//expects pct in [0..1]
void LoadingScene::progress_CB(float pct)
{
	//CCLOG("%f%%", pct * 100);

	LoadingUI* ui = getInterface<LoadingUI>(LoadingUI::id);
	if (ui)
	{
		//cocos2d::ui::Widget* loadbarpnl = ui->getRoot()->getChildByName("PNL_LoadingBar");
		//if (loadbarpnl)
		//{
			ui::Widget* loadbarw = ui->getRoot()->getChildByName("LoadingBar"); //loadbarpnl->getChildByName("LoadingBar");
			ui::LoadingBar* loadbar = dynamic_cast<ui::LoadingBar*>(loadbarw);

			if (loadbar)
			{
				loadbar->setPercent(static_cast<int>(pct * 100));
			}
		//}
	}

	if (pct >= 1.0f && !m_loadDoneCB_called)
	{
		//DOWNloading is finished.
		//We should clean the pathCache that was computed before download
		FileUtils::getInstance()->purgeCachedEntries();

		//loading finished. lets move on.
		m_loadDoneCB_called = true;
		m_loadDoneCB();
	}
}

void LoadingScene::error_CB()
{
	CCLOGERROR("ERROR");

	//Error UI
	ErrorUI* errorui = getInterface<ErrorUI>(ErrorUI::id);
	errorui->activate();

	//m_loadingManager.~LoadingManager();
	m_loadMan_del_scheduled = true;

}