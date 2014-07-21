#include "WkCocosApp/LoadingScene.h"

#include "WkCocosApp/LoadingUI.h"

#include "WkCocosApp/HelloWorldScene.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <numeric>

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	// add a layer
	cocos2d::Layer* newLayer = cocos2d::Layer::create();
	addChild(newLayer);

	//Load UI
	LoadingUI* loadui = new LoadingUI();
	loadui->getRoot()->setEnabled(true);
	loadui->getRoot()->setVisible(true);
	newLayer->addChild(loadui->getRoot());
	m_ui[LoadingUI::id] = loadui;

	m_downloadManager.start();

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
	m_preloadManager.addDataLoad(respath);
}

void LoadingScene::setLoadDoneCallback(std::function<void()> cb)
{
	m_loadDoneCB = cb;
}

void LoadingScene::scheduleDLCCheck()
{
	m_downloadManager.addDataDownload("manifest.json");
}

void LoadingScene::update(float delta)
{
	//if callback was called, loading is finished.
	if (!m_loadDoneCB_called)
	{
		m_downloadManager.step(delta);
	}
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
			cocos2d::ui::Widget* loadbarw = ui->getRoot()->getChildByName("LoadingBar"); //loadbarpnl->getChildByName("LoadingBar");
			cocos2d::ui::LoadingBar* loadbar = dynamic_cast<cocos2d::ui::LoadingBar*>(loadbarw);

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
		cocos2d::FileUtils::getInstance()->purgeCachedEntries();

		//loading finished. lets move on.
		m_loadDoneCB_called = true;
		m_loadDoneCB();
	}
}
