#include "WkCocosApp/ErrorUI.h"

#include "cocos/ui/CocosGUI.h"

#include "cocos2d.h"

USING_NS_CC;

const std::string ErrorUI::id = "error";

ErrorUI::ErrorUI() : Interface()
{
	//building UI hierarchy
	m_widget = ui::Layout::create();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// position the label on the center of the screen

	auto label = LabelTTF::create("ERROR", "Arial", 48);
	label->setPosition(Vec2(
		visibleSize.width / 2,
		(visibleSize.height + label->getContentSize().height) / 2));
	m_widget->addChild(label);

	m_refreshButton = ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_refreshButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::refreshCallback, this));
	m_refreshButton->setPosition(Vec2(
		(visibleSize.width - m_refreshButton->getContentSize().width) / 2,
		(visibleSize.height - m_refreshButton->getContentSize().height) / 2));
	m_widget->addChild(m_refreshButton);

	m_skipButton = ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_skipButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::skipCallback, this));
	m_skipButton->setPosition(Vec2(
		(visibleSize.width + m_skipButton->getContentSize().width) / 2,
		(visibleSize.height - m_skipButton->getContentSize().height) / 2));
	m_widget->addChild(m_skipButton);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, ui::Widget*>(id, m_widget));
	}

}

void ErrorUI::activate()
{
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

void ErrorUI::refreshCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		m_refreshCB();
	}
}

void ErrorUI::skipCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
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