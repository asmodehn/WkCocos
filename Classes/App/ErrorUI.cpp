#include "WkCocosApp/ErrorUI.h"

//#include "WkCocosApp/HelloWorldScene.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

const std::string ErrorUI::id = "error";

ErrorUI::ErrorUI(bool *parentError)
: Interface(), m_parentError(parentError)
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

	m_refreshButton = cocos2d::ui::Button::create(
		"RefreshNormal.png",
		"RefreshSelected.png",
		"RefreshNormal.png"
	);
	m_refreshButton->addTouchEventListener(CC_CALLBACK_2(ErrorUI::refreshCallback, this));
	m_refreshButton->setPosition(Vec2(
		visibleSize.width / 2,
		(visibleSize.height - m_refreshButton->getContentSize().height) / 2));
	m_widget->addChild(m_refreshButton);

	//auto menu = Menu::create(refreshButton, NULL);
	//menu->setPosition(Vec2::ZERO);
	//m_widget->addChild(menu, 1);
		
	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
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

void ErrorUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		*m_parentError = true;
	}
}