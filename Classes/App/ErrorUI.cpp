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
		m_label = cocos2d::ui::Text::create("ERROR", "Thonburi", 24);
		m_label->setPosition(cocos2d::Vec2(0, m_label->getContentSize().height / 2));
		m_widget->addChild(m_label, 2);

		m_number = cocos2d::ui::Text::create("0", "Thonburi", 24);
		//m_number->setPosition(cocos2d::Vec2(-widgetSize.width / 2 + 20, widgetSize.height / 2 - 20));
		m_widget->addChild(m_number, 2);

		m_refreshButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
		m_refreshButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::refreshCallback, this));
		m_refreshButton->setPosition(cocos2d::Vec2(-m_refreshButton->getContentSize().width / 2, -m_refreshButton->getContentSize().height / 2));
		m_widget->addChild(m_refreshButton, 2);

		m_skipButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_skipButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::skipCallback, this));
		m_skipButton->setPosition(cocos2d::Vec2(m_skipButton->getContentSize().width / 2, -m_skipButton->getContentSize().height / 2));
		m_widget->addChild(m_skipButton, 2);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

		m_widget->setName("error_ui");
	}
}

void ErrorUI::activate(std::string msg)
{
	int errors = 0;
	auto parent = m_widget->getParent();
	parent->enumerateChildren("error_ui", [&errors](cocos2d::Node *node) // can be done without previous line if correct string passed here
	{
		++errors;
		return false;
	});
	
	m_number->setString(WkCocos::ToolBox::itoa(errors));
	m_label->setString(msg);
	
	m_widget->setContentSize(cocos2d::Size(m_label->getContentSize().width + 40, m_label->getContentSize().height + 120));
	cocos2d::Size widgetSize = m_widget->getContentSize();
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	m_number->setPositionX(-widgetSize.width / 2 + 20);
	m_number->setPositionY(widgetSize.height / 2 - 24);

	m_skipButton->setPositionY(-(m_label->getContentSize().height + m_skipButton->getContentSize().height) / 2);
	m_refreshButton->setPositionY(-(m_label->getContentSize().height + m_refreshButton->getContentSize().height) / 2);

	auto fadeBack = cocos2d::DrawNode::create();
	auto errorBack = cocos2d::DrawNode::create();

	fadeBack->drawTriangle(cocos2d::Vec2(-visibleSize.width / 2, -visibleSize.height / 2), cocos2d::Vec2(visibleSize.width / 2, -visibleSize.height / 2),
		cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2), cocos2d::Color4F(0, 0, 0, 0.6f));

	fadeBack->drawTriangle(cocos2d::Vec2(-visibleSize.width / 2, -visibleSize.height / 2), cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2),
		cocos2d::Vec2(-visibleSize.width / 2, visibleSize.height / 2), cocos2d::Color4F(0, 0, 0, 0.6f));

	fadeBack->setPositionY(visibleSize.height / 2 - m_widget->getPositionY());
	
	errorBack->drawTriangle(cocos2d::Vec2(-widgetSize.width / 2, -widgetSize.height / 2), cocos2d::Vec2(widgetSize.width / 2, -widgetSize.height / 2),
		cocos2d::Vec2(widgetSize.width / 2, widgetSize.height / 2), cocos2d::Color4F(0.3f, 0.3f, 0.3f, 1));

	errorBack->drawTriangle(cocos2d::Vec2(-widgetSize.width / 2, -widgetSize.height / 2), cocos2d::Vec2(widgetSize.width / 2, widgetSize.height / 2),
		cocos2d::Vec2(-widgetSize.width / 2, widgetSize.height / 2), cocos2d::Color4F(0.3f, 0.3f, 0.3f, 1));

	m_widget->addChild(fadeBack, 1);
	m_widget->addChild(errorBack, 1);

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