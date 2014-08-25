#include "WkCocosApp/ErrorUI.h"

#include "WkCocosApp/GameLogic.h"
#include "cocos/ui/CocosGUI.h"

#include "cocos2d.h"

const std::string ErrorUI::id = "error";

ErrorUI::ErrorUI() : Interface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width / 2, visibleSize.height / 2));
		cocos2d::Size widgetSize = m_widget->getContentSize();

		m_label = cocos2d::ui::Text::create("ERROR", "Arial", 24);
		m_label->setPosition(cocos2d::Vec2(0, m_label->getContentSize().height / 2));
		m_widget->addChild(m_label);

		m_refreshButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
		m_refreshButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::refreshCallback, this));
		m_refreshButton->setPosition(cocos2d::Vec2(-m_refreshButton->getContentSize().width / 2, -m_refreshButton->getContentSize().height / 2));
		m_widget->addChild(m_refreshButton);

		m_skipButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_skipButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::skipCallback, this));
		m_skipButton->setPosition(cocos2d::Vec2(m_skipButton->getContentSize().width / 2, -m_skipButton->getContentSize().height / 2));
		m_widget->addChild(m_skipButton);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
	GameLogic::Instance().getPlayer().getLocalDatamgr()->getEventManager()->subscribe<WkCocos::LocalData::Events::Error>(*this);
}

void ErrorUI::receive(const WkCocos::LocalData::Events::Error &LD)
{
	m_label->setText(LD.msg);
	m_widget->setVisible(true);
	m_widget->setEnabled(true);
}

void ErrorUI::activate(std::string msg)
{
	m_label->setText(msg);
	m_widget->setVisible(true);
	m_widget->setEnabled(true);
}

void ErrorUI::deactivate()
{
	m_widget->setVisible(false);
	m_widget->setEnabled(false);
}

ErrorUI::~ErrorUI()
{}

void ErrorUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		m_refreshCB();
	}
}

void ErrorUI::skipCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		m_skipCB();
	}
}

void ErrorUI::setRefreshCallback(std::function<void()> cb)
{
	m_refreshCB = cb;
}

void ErrorUI::setSkipCallback(std::function<void()> cb)
{
	m_skipCB = cb;
}