#include "WkCocosApp/SavingUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;

const std::string SavingUI::id = "saving";

SavingUI::SavingUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	//m_filepath = id;

	//building UI hierarchy
	m_widget = ui::Layout::create();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_saveButton = ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_saveButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::saveCallback, this));
	m_saveButton->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.5));
	m_widget->addChild(m_saveButton);

	m_saveLabel = ui::Text::create("SAVE", "Arial", 21);
	m_saveLabel->setPosition(m_saveButton->getPosition() + Vec2(0, m_saveButton->getContentSize().height));
	m_widget->addChild(m_saveLabel);

	m_loadButton = ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_loadButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::loadCallback, this));
	m_loadButton->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.75));
	m_widget->addChild(m_loadButton);

	m_loadLabel = ui::Text::create("LOAD", "Arial", 21);
	m_loadLabel->setPosition(m_loadButton->getPosition() + Vec2(0, m_loadButton->getContentSize().height));
	m_widget->addChild(m_loadLabel);
	
	m_gemLabel = ui::Text::create("? GEM", "Arial", 21);
	m_gemLabel->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.75));
	m_widget->addChild(m_gemLabel);

	m_goldLabel = ui::Text::create("? GOLD", "Arial", 21);
	m_goldLabel->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
	m_widget->addChild(m_goldLabel);
	
	m_gemrandButton = ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_gemrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::gemrandCallback, this));
	m_gemrandButton->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.75));
	m_widget->addChild(m_gemrandButton);

	m_goldrandButton = ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_goldrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::goldrandCallback, this));
	m_goldrandButton->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.5));
	m_widget->addChild(m_goldrandButton);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, ui::Widget*>(id, m_widget));
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

void SavingUI::saveCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("SAVE BUTTON CLICKED");

		GameLogic::Instance().getPlayer().saveData();

	}
}


void SavingUI::loadCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("LOAD BUTTON CLICKED");

		GameLogic::Instance().getPlayer().loadData();

	}
}

void SavingUI::goldrandCallback(Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GOLD RAND BUTTON CLICKED");

		GameLogic::Instance().getPlayer().m_gold = rand() % 10000;

	}
}


void SavingUI::gemrandCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GEM RAND BUTTON CLICKED");

		GameLogic::Instance().getPlayer().m_gem = rand() % 100;

	}
}


