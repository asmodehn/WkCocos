#include "WkCocosApp/NavUI.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string NavUI::id = "Navi";

NavUI::NavUI(entityx::ptr<entityx::EventManager> events)
	: Interface()
	, event_manager(events)
{
	//building UI
	m_widget = cocos2d::ui::Layout::create();

	//The NavUI should not take more than a quarter of the visible size
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height / 4));
	cocos2d::Size widgetSize = m_widget->getContentSize();

	m_prevButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_prevButton->addTouchEventListener(std::bind(&NavUI::prevCallback, this, std::placeholders::_1, std::placeholders::_2));
	m_prevButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, 0));
	m_widget->addChild(m_prevButton);

	m_prevLabel = cocos2d::ui::Text::create("PREV", "Arial", 21);
	m_prevLabel->setPosition(m_prevButton->getPosition() + cocos2d::Vec2(0, m_prevButton->getContentSize().height));
	m_widget->addChild(m_prevLabel);

	m_nextButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_nextButton->addTouchEventListener(std::bind(&NavUI::nextCallback, this, std::placeholders::_1, std::placeholders::_2));
	m_nextButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, 0));
	m_widget->addChild(m_nextButton);

	m_nextLabel = cocos2d::ui::Text::create("NEXT", "Arial", 21);
	m_nextLabel->setPosition(m_nextButton->getPosition() + cocos2d::Vec2(0, m_nextButton->getContentSize().height));
	m_widget->addChild(m_nextLabel);
	
	m_titleLabel = cocos2d::ui::Text::create("ID", "Arial", 21);
	m_titleLabel->setPosition((m_nextButton->getPosition() + m_prevButton->getPosition()) /2);
	m_widget->addChild(m_titleLabel);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}

}

NavUI::~NavUI()
{}

void NavUI::nextCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("NEXT BUTTON CLICKED");
		event_manager->emit<Next>();
	}
}


void NavUI::prevCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("PREV BUTTON CLICKED");
		event_manager->emit<Prev>();
	}
}

void NavUI::setTitle(std::string title)
{
	m_titleLabel->setText(title);
}

