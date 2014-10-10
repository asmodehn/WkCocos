#include "WkCocosApp/TimerUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string TimerUI::id = "timer";

TimerUI::TimerUI()
	: Interface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width / 2, visibleSize.height / 2));
		cocos2d::Size widgetSize = m_widget->getContentSize();

		m_startButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_startButton->addTouchEventListener(CC_CALLBACK_2(TimerUI::startCallback, this));
		m_startButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, 0));
		m_widget->addChild(m_startButton);

		m_startLabel = cocos2d::ui::Text::create("START", "Arial", 21);
		m_startLabel->setPosition(m_startButton->getPosition() + cocos2d::Vec2(0, m_startButton->getContentSize().height));
		m_widget->addChild(m_startLabel);

		m_stopButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_stopButton->addTouchEventListener(CC_CALLBACK_2(TimerUI::stopCallback, this));
		m_stopButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, 0));
		m_widget->addChild(m_stopButton);

		m_stopLabel = cocos2d::ui::Text::create("STOP", "Arial", 21);
		m_stopLabel->setPosition(m_stopButton->getPosition() + cocos2d::Vec2(0, m_stopButton->getContentSize().height));
		m_widget->addChild(m_stopLabel);
	
		m_countLabel = cocos2d::ui::Text::create("", "Arial", 21);
		m_widget->addChild(m_countLabel);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
	g_gameLogic->getPlayer().getTimermgr()->getEventManager()->subscribe<WkCocos::Timer::Events::TimerUpdate>(*this);
	g_gameLogic->getPlayer().getTimermgr()->getEventManager()->subscribe<WkCocos::Timer::Events::AlarmOff>(*this);
}

TimerUI::~TimerUI()
{}

void TimerUI::receive(const WkCocos::Timer::Events::TimerUpdate &tu)
{
	m_countLabel->setText(WkCocos::ToolBox::itoa(tu.getTimeLeft()) + " sec");
}

void TimerUI::receive(const WkCocos::Timer::Events::AlarmOff &ao)
{
	m_countLabel->setText(ao.id + " alarm off");
}

void TimerUI::startCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("START BUTTON CLICKED");
		
		g_gameLogic->getPlayer().setTimer("testing", 65);
		
	}
}


void TimerUI::stopCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("STOP BUTTON CLICKED");

		g_gameLogic->getPlayer().stopTimer("testing");
	}
}

