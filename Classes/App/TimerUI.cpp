#include "WkCocosApp/TimerUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

const std::string TimerUI::id = "timer";

TimerUI::TimerUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	m_startButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_startButton->addTouchEventListener(CC_CALLBACK_2(TimerUI::startCallback, this));
	m_startButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_startButton->getContentSize().width) / 4,
		(visibleSize.height - m_startButton->getContentSize().height) / 3
	));
	m_widget->addChild(m_startButton);
	m_startLabel = cocos2d::ui::Text::create("START", "Arial", 21);
	m_startLabel->setPosition(m_startButton->getPosition() + cocos2d::Vec2(0, m_startButton->getContentSize().height));
	m_widget->addChild(m_startLabel);


	m_stopButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_stopButton->addTouchEventListener(CC_CALLBACK_2(TimerUI::stopCallback, this));
	m_stopButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_stopButton->getContentSize().width) / 4,
		(visibleSize.height - m_stopButton->getContentSize().height) * 2 / 3
		));
	m_widget->addChild(m_stopButton);
	m_stopLabel = cocos2d::ui::Text::create("STOP", "Arial", 21);
	m_stopLabel->setPosition(m_stopButton->getPosition() + cocos2d::Vec2(0, m_stopButton->getContentSize().height));
	m_widget->addChild(m_stopLabel);
	
	m_countLabel = cocos2d::ui::Text::create("? GEM", "Arial", 21);
	m_countLabel->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_startButton->getContentSize().width) * 2/ 4,
		(visibleSize.height - m_startButton->getContentSize().height) * 2/ 3
		));
	m_widget->addChild(m_countLabel);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

TimerUI::~TimerUI()
{}

void TimerUI::update(float delta)
{
	m_countLabel->setText(WkCocos::ToolBox::itoa(GameLogic::Instance().getPlayer().m_gem) );
}

void TimerUI::startCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("START BUTTON CLICKED");

		GameLogic::Instance().getPlayer().saveData();
	}
}


void TimerUI::stopCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("STOP BUTTON CLICKED");

		GameLogic::Instance().getPlayer().loadData();
	}
}

