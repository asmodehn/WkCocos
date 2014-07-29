#include "WkCocosApp/SavingUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

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
		(visibleSize.width - m_saveButton->getContentSize().width) / 4,
		(visibleSize.height - m_saveButton->getContentSize().height) / 3
	));
	m_widget->addChild(m_saveButton);
	m_saveLabel = cocos2d::ui::Text::create("SAVE", "Arial", 21);
	m_saveLabel->setPosition(m_saveButton->getPosition() + cocos2d::Vec2(0, m_saveButton->getContentSize().height ));
	m_widget->addChild(m_saveLabel);


	m_loadButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_loadButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::loadCallback, this));
	m_loadButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_saveButton->getContentSize().width) / 4,
		(visibleSize.height - m_saveButton->getContentSize().height) * 2 / 3
		));
	m_widget->addChild(m_loadButton);
	m_loadLabel = cocos2d::ui::Text::create("LOAD", "Arial", 21);
	m_loadLabel->setPosition(m_loadButton->getPosition() + cocos2d::Vec2(0, m_loadButton->getContentSize().height));
	m_widget->addChild(m_loadLabel);
	
	m_gemLabel = cocos2d::ui::Text::create("? GEM", "Arial", 21);
	m_gemLabel->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_saveButton->getContentSize().width) * 2/ 4,
		(visibleSize.height - m_saveButton->getContentSize().height) * 2/ 3
		));
	m_widget->addChild(m_gemLabel);

	m_goldLabel = cocos2d::ui::Text::create("? GOLD", "Arial", 21);
	m_goldLabel->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_saveButton->getContentSize().width) * 2/ 4,
		(visibleSize.height - m_saveButton->getContentSize().height) / 3
		));
	m_widget->addChild(m_goldLabel);
	
	m_gemrandButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_gemrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::gemrandCallback, this));
	m_gemrandButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_saveButton->getContentSize().width) * 3/ 4,
		(visibleSize.height - m_saveButton->getContentSize().height) * 2 / 3
		));
	m_widget->addChild(m_gemrandButton);

	m_goldrandButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_goldrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::goldrandCallback, this));
	m_goldrandButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_saveButton->getContentSize().width) * 3/ 4,
		(visibleSize.height - m_saveButton->getContentSize().height) * 1 / 3
		));
	m_widget->addChild(m_goldrandButton);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

SavingUI::~SavingUI()
{}

void SavingUI::update(float delta)
{
	if (m_gemLabel->isVisible())
	{
		m_gemLabel->setText(WkCocos::ToolBox::itoa(GameLogic::Instance().getPlayer().m_gem) + " GEM");
	}
	if (m_goldLabel->isVisible())
	{
		m_goldLabel->setText(WkCocos::ToolBox::itoa(GameLogic::Instance().getPlayer().m_gold) + " GOLD");
	}
}

void SavingUI::saveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("SAVE BUTTON CLICKED");

		GameLogic::Instance().getPlayer().saveData();
	}
}


void SavingUI::loadCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("LOAD BUTTON CLICKED");

		GameLogic::Instance().getPlayer().loadData();
	}
}

void SavingUI::goldrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GOLD RAND BUTTON CLICKED");
		GameLogic::Instance().getPlayer().m_gold = rand() % 10000;

	}
}


void SavingUI::gemrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GEM RAND BUTTON CLICKED");
		GameLogic::Instance().getPlayer().m_gem = rand() % 100;
	}
}


