#include "WkCocosApp/SavingUI.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

const std::string SavingUI::id = "saving";

SavingUI::SavingUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	m_saveButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_saveButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::saveCallback, this));
	m_saveButton->setPosition(cocos2d::Vec2(
		(visibleSize.width + m_saveButton->getContentSize().width) / 2,
		(visibleSize.height - m_saveButton->getContentSize().height) / 2));
	m_widget->addChild(m_saveButton);

	
	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

SavingUI::~SavingUI()
{}

void SavingUI::saveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	CCLOG("BUTTON CLICKED");
}

