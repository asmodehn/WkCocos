#include "WkCocosApp/TestScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/ShopUI.h"
#include "WkCocosApp/TimerUI.h"
#include "WkCocosApp/DownloadUI.h"
#include "WkCocosApp/ErrorUI.h"
#include "WkCocosApp/PlayersListUI.h"

#include "WkCocosApp/GameLogic.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <numeric>

TestScene::TestScene()
: Scene()
, ui_event_manager(entityx::EventManager::make())
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

	//Navigation UI
	NavUI* navui = new NavUI(ui_event_manager);
	ui_event_manager->subscribe<NavUI::Next>(*this);
	ui_event_manager->subscribe<NavUI::Prev>(*this);

	auto navroot = navui->getRoot();
	navroot->setEnabled(true);
	navroot->setVisible(true);
	addChild(navroot);
	navroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.17f));
	m_ui[NavUI::id] = navui;

	//Saving UI
	SavingUI* saveui = new SavingUI();
	auto saveroot = saveui->getRoot();
	saveroot->setEnabled(false);
	saveroot->setVisible(false);
	addChild(saveroot);
	saveroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	m_ui[SavingUI::id] = saveui;

	//TimerUI
	TimerUI* timerui = new TimerUI();
	auto timerroot = timerui->getRoot();
	timerroot->setEnabled(false);
	timerroot->setVisible(false);
	addChild(timerroot);
	timerroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	m_ui[TimerUI::id] = timerui;
	timerroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.75, visibleSize.height * 0.75));
	//*/

	//PlayersListUI
	PlayersListUI* playerslistui = new PlayersListUI();
	auto playerslistroot = playerslistui->getRoot();
	playerslistroot->setEnabled(false);
	playerslistroot->setVisible(false);
	addChild(playerslistroot);
	m_ui[PlayersListUI::id] = playerslistui;
	playerslistroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
	//*/

	//Error UI
	ErrorUI* errorui = new ErrorUI();
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	errorroot->setEnabled(false);
	errorroot->setVisible(false);
	m_ui[ErrorUI::id] = errorui;
	errorroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.75, visibleSize.height * 0.25));

	errorui->setRefreshCallback([this, errorui](){
		
		errorui->deactivate();

	});

	errorui->setSkipCallback([this, errorui](){

		errorui->deactivate();

	});
	//*/
	
	//
	auto sprite = cocos2d::Sprite::create("HelloWorld.png");
	sprite->setScaleX((visibleSize.width / 2 - closeItem->getContentSize().width) / sprite->getContentSize().width);
	sprite->setScaleY((visibleSize.height / 2 - closeItem->getContentSize().height) / sprite->getContentSize().height);
	sprite->setPosition(cocos2d::Vec2((visibleSize.width / 2 + closeItem->getPositionX() - closeItem->getContentSize().width / 2) / 2,
		(visibleSize.height / 2 + closeItem->getPositionY() + closeItem->getContentSize().height / 2) / 2));
	addChild(sprite, 0);

	sprite->setVisible(false);
	//*/

	//ShopUI
	ShopUI* shopui = new ShopUI();
	shopui->getRoot()->setEnabled(false);
	shopui->getRoot()->setVisible(false);
	addChild(shopui->getRoot());
	shopui->getRoot()->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	m_ui[ShopUI::id] = shopui;

	//DownloadUI
	DownloadUI* dlui = new DownloadUI();
	dlui->getRoot()->setEnabled(false);
	dlui->getRoot()->setVisible(false);
	addChild(dlui->getRoot());
	dlui->getRoot()->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	m_ui[DownloadUI::id] = dlui;

	//activating first UI : 
	saveroot->setEnabled(true);
	saveroot->setVisible(true);
	currentUI = SavingUI::id;
	navui->setTitle(currentUI);
	
	m_time = cocos2d::ui::Text::create("", "Arial", 20);
	m_time->setPosition(cocos2d::Vec2(sprite->getPositionX(), closeItem->getPositionY()));
	CCLOG("\nTimeX=%d TimeY=%d", sprite->getPositionX(), closeItem->getPositionY());
	addChild(m_time);

	GameLogic::Instance().getPlayer().getOnlineDatamgr()->getEventManager()->subscribe<WkCocos::OnlineData::Events::ServerTime>(*this);
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
	if (!m_waiting_for_server_time)
	{
		GameLogic::Instance().getPlayer().getServerTime();
		m_waiting_for_server_time = true;
	}

	Scene::update(delta);
}


void TestScene::receive(const NavUI::Next &nxt)
{
	auto cur = m_ui.find(currentUI);

	//if we canot find current UI we start again from beginning
	if (cur == m_ui.end())
	{
		cur = m_ui.begin();
	}
	auto uiroot = cur->second->getRoot();
	uiroot->setEnabled(false);
	uiroot->setVisible(false);

	do
	{
		if (++cur == m_ui.end())
		{
			cur = m_ui.begin();
		}
	} while (cur->first == NavUI::id);
	
	uiroot = cur->second->getRoot();
	uiroot->setEnabled(true);
	uiroot->setVisible(true);
	currentUI = cur->first;

	NavUI* nav = getInterface<NavUI>(NavUI::id);
	nav->setTitle(currentUI);
}

void TestScene::receive(const NavUI::Prev &prv)
{
	auto cur = m_ui.find(currentUI);

	//if we canot find current UI we start again from beginning
	if (cur == m_ui.end())
	{
		cur = m_ui.begin();
	}

	auto uiroot = cur->second->getRoot();
	uiroot->setEnabled(false);
	uiroot->setVisible(false);

	//reversing iterator
	auto rcur = std::map<std::string, WkCocos::Interface*>::reverse_iterator(cur);
	//check for rend after conversion
	if (rcur == m_ui.rend())
	{
		rcur = m_ui.rbegin();
	}

	while (rcur->first == NavUI::id)
	{
		if (++rcur == m_ui.rend())
		{
			rcur = m_ui.rbegin();
		}
	}

	uiroot = rcur->second->getRoot();
	uiroot->setEnabled(true);
	uiroot->setVisible(true);
	currentUI = rcur->first;

	NavUI* nav = getInterface<NavUI>(NavUI::id);
	nav->setTitle(currentUI);
}


void TestScene::receive(const WkCocos::OnlineData::Events::ServerTime &st)
{
	m_waiting_for_server_time = false;
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
	{
		m_time->setText(WkCocos::ToolBox::itoa(st.serverTime.tm_hour) + ":" + WkCocos::ToolBox::itoa(st.serverTime.tm_min) + ":" + WkCocos::ToolBox::itoa(st.serverTime.tm_sec));
	});
}

void TestScene::error_CB(std::string msg)
{
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	//Error UI. created only when we need it.
	ErrorUI* errorui = new ErrorUI();
	auto errorroot = errorui->getRoot();
	addChild(errorroot);
	m_ui[ErrorUI::id] = errorui;
	errorroot->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));

	errorui->setRefreshCallback([this, errorui](){
		errorui->deactivate();
		//removing UI from scene
		removeChild(errorui->getRoot());
		m_ui.erase(ErrorUI::id);
		//displaying navi
		m_ui[NavUI::id]->getRoot()->setEnabled(true);
		m_ui[NavUI::id]->getRoot()->setVisible(true);

		NavUI* nav = getInterface<NavUI>(NavUI::id);
		nav->setTitle(NavUI::id);
	});

	errorui->setSkipCallback([this, errorui](){
		errorui->deactivate();
		//removing UI from scene
		removeChild(errorui->getRoot());
		m_ui.erase(ErrorUI::id);
		//displaying navi
		m_ui[NavUI::id]->getRoot()->setEnabled(true);
		m_ui[NavUI::id]->getRoot()->setVisible(true);

		NavUI* nav = getInterface<NavUI>(NavUI::id);
		nav->setTitle(NavUI::id);
	});

	errorui->activate(msg);
	
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