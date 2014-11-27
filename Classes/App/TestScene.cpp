#include "WkCocosApp/TestScene.h"

#include "WkCocosApp/SavingUI.h"
#include "WkCocosApp/ShopUI.h"
#include "WkCocosApp/TimerUI.h"
#include "WkCocosApp/DownloadUI.h"
#include "WkCocosApp/ErrorUI.h"
#include "WkCocosApp/PlayersListUI.h"
#include "WkCocosApp/DocsListUI.h"
#include "WkCocosApp/WebUI.h"
#include "WkCocosApp/LogUI.h"
#include "WkCocosApp/GameUI.h"
#include "WkCocosApp/GameLogic.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <numeric>

TestScene::TestScene()
: Scene()
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
	//cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(TestScene::menuCloseCallback, this));
	closeItem->setPosition(cocos2d::Vec2(visibleSize.width - closeItem->getContentSize().width / 2,
		visibleSize.height - closeItem->getContentSize().height / 2));
	auto menu = cocos2d::Menu::create(closeItem, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	addChild(menu, 1);

	m_prevButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_prevButton->addTouchEventListener(CC_CALLBACK_2(TestScene::prevCallback, this));
	m_prevButton->setPosition(cocos2d::Vec2(m_prevButton->getContentSize().width / 2, m_prevButton->getContentSize().height / 2));
	m_prevButton->setScaleX(-1);
	addChild(m_prevButton);

	m_prevLabel = cocos2d::ui::Text::create("PREV", "Thonburi", 21);
	m_prevLabel->setPosition(m_prevButton->getPosition() + cocos2d::Vec2(m_prevButton->getContentSize().width + m_prevLabel->getContentSize().width, 0));
	addChild(m_prevLabel);

	m_nextButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_nextButton->addTouchEventListener(CC_CALLBACK_2(TestScene::nextCallback, this));
	m_nextButton->setPosition(cocos2d::Vec2(visibleSize.width - m_nextButton->getContentSize().width / 2, m_prevButton->getContentSize().height / 2));
	addChild(m_nextButton);

	m_nextLabel = cocos2d::ui::Text::create("NEXT", "Thonburi", 21);
	m_nextLabel->setPosition(m_nextButton->getPosition() - cocos2d::Vec2(m_nextLabel->getContentSize().width + m_nextButton->getContentSize().width, 0));
	addChild(m_nextLabel);

	m_titleLabel = cocos2d::ui::Text::create("ID", "Thonburi", 21);
	m_titleLabel->setPosition((m_nextButton->getPosition() + m_prevButton->getPosition()) / 2);
	addChild(m_titleLabel);

	//Saving UI
	SavingUI* saveui = new SavingUI();
	saveui->setEnabled(false);
	saveui->setVisible(false);
	addInterface(SavingUI::id, saveui);
	saveui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	//*/

	//TimerUI
	TimerUI* timerui = new TimerUI();
	timerui->setEnabled(false);
	timerui->setVisible(false);
	addInterface(TimerUI::id, timerui);
	timerui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	//*/

	//PlayersListUI
	PlayersListUI* playerslistui = new PlayersListUI();
	playerslistui->setEnabled(false);
	playerslistui->setVisible(false);
	addInterface(PlayersListUI::id, playerslistui);
	playerslistui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	//*/

	//DocsListUI
	DocsListUI* docslistui = new DocsListUI();
	docslistui->setEnabled(false);
	docslistui->setVisible(false);
	addInterface(DocsListUI::id, docslistui);
	docslistui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	//*/

	//ShopUI
	ShopUI* shopui = new ShopUI();
	shopui->setEnabled(false);
	shopui->setVisible(false);
	addInterface(ShopUI::id, shopui);
	shopui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));
	//*/

	//DownloadUI
	DownloadUI* dlui = new DownloadUI();
	dlui->setEnabled(false);
	dlui->setVisible(false);
	addInterface(DownloadUI::id, dlui);
	dlui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	//*/

	//WebUI
	WebUI* webui = new WebUI();
	webui->setEnabled(false);
	webui->setVisible(false);
	addInterface(WebUI::id, webui);
	webui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	//*/

	//LogUI
	LogUI* logui = new LogUI();
	logui->setEnabled(false);
	logui->setVisible(false);
	addInterface(LogUI::id, logui);
	logui->setPosition(cocos2d::Vec2(0, 40));
	//*/

	/*//GameUI
	GameUI* gameui = new GameUI();
	gameui->setEnabled(false);
	gameui->setVisible(false);
	addInterface(GameUI::id, gameui);
	gameui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	//*/

	//activating first UI : 
	saveui->setEnabled(true);
	saveui->setVisible(true);
	currentUI = SavingUI::id;
	m_titleLabel->setString(currentUI);
	
	m_time = cocos2d::ui::Text::create("", "Thonburi", 20);
	m_time->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, /*closeItem->getPositionY()*/ visibleSize.height - 20));
	addChild(m_time);

	g_gameLogic->getOnlineDataManager().getEventManager()->subscribe<WkCocos::OnlineData::Events::Error>(*this);
	g_gameLogic->getLocalDataManager().getEventManager()->subscribe<WkCocos::LocalData::Events::Error>(*this);
	g_gameLogic->getPlayer().getEventManager()->subscribe<MyPlayer::Error>(*this);
	
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
	m_time->setString(WkCocos::ToolBox::itoa(g_gameLogic->getPlayer().getTimermgr()->getServerLocalTime().tm_hour) + ":" +
		WkCocos::ToolBox::itoa(g_gameLogic->getPlayer().getTimermgr()->getServerLocalTime().tm_min) + ":" +
		WkCocos::ToolBox::itoa(g_gameLogic->getPlayer().getTimermgr()->getServerLocalTime().tm_sec));
	Scene::update(delta);
}

void TestScene::nextCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto cur = m_ui.find(currentUI);

		cur->second->setEnabled(false);
		cur->second->setVisible(false);

		if (++cur == m_ui.end())
			cur = m_ui.begin();

		cur->second->setEnabled(true);
		cur->second->setVisible(true);
		currentUI = cur->first;
		m_titleLabel->setString(currentUI);
	}
}

void TestScene::prevCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto cur = m_ui.find(currentUI);

		cur->second->setEnabled(false);
		cur->second->setVisible(false);

		if (cur == m_ui.begin())
			cur = m_ui.end();

		--cur;

		cur->second->setEnabled(true);
		cur->second->setVisible(true);
		currentUI = cur->first;
		m_titleLabel->setString(currentUI);
	}
}

void TestScene::receive(const WkCocos::LocalData::Events::Error &LD)
{
	/**
	* Handling all possible Local Errors here
	*/
	//if (LD.httpErrorCode == 4 && LD.app42ErrorCode == 56)
	{

		error_CB(LD.msg
			, [=](){
		}
			, [=](){
		});
	}
}

void TestScene::receive(const WkCocos::OnlineData::Events::Error &OD)
{
	std::stringstream errormsg;
	errormsg << "HTTP " << OD.httpErrorCode << std::endl;
	errormsg << "App42 " << OD.app42ErrorCode << std::endl;
	errormsg << OD.errorMessage << std::endl;
	errormsg << OD.errorDetails;

	/**
	* Handling all possible Online Errors here
	*/
	if (OD.httpErrorCode == -1 && OD.app42ErrorCode == 0) 
	{	// could not resolve host : api.shephertz.com; Host not found
		// unknown SSL protocol error in connection to api.shephertz.com:443
		error_CB(errormsg.str()
			,[=](){

		}
			,[=](){

		});
	}
	else if (OD.httpErrorCode == -1 && OD.app42ErrorCode == 0)
	{
	}
	else if (!OD.errorDetails.compare("docs list"))
	{
		error_CB(errormsg.str()
			, [=](){

		}
			, [=](){

		});
	}
	else if (!OD.errorDetails.compare("get data by query"))
	{
		error_CB(errormsg.str()
			, [=](){

		}
			, [=](){

		});
	}
}

void TestScene::receive(const MyPlayer::Error &player_error)
{
	std::stringstream errormsg;
	errormsg << player_error.message;
	/**
	* Handling all possible Online Errors here
	*/
	error_CB(errormsg.str()
		, [=](){

	}
		, [=](){

	});
	
}

void TestScene::error_CB(std::string msg, std::function<void()> retryCB, std::function<void()> skipCB)
{
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	ErrorUI* errorui = new ErrorUI();
	getUIView()->addChild(errorui->getRoot(), 2); // we do not want to addInterface to the list of UI in scene ( multiple errors UI can appear and will cause problems ).
	errorui->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.67f));

	errorui->setRefreshCallback([this, errorui, retryCB](){
		if (retryCB) retryCB();
		errorui->deactivate();
		getUIView()->removeChild(errorui->getRoot()); //garbage collected UI
	});

	errorui->setSkipCallback([this, errorui, skipCB](){
		if (skipCB) skipCB();
		errorui->deactivate();
		getUIView()->removeChild(errorui->getRoot()); //garbage collected UI
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