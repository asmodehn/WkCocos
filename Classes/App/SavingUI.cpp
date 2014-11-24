#include "WkCocosApp/SavingUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/GUI/CCEditBox/CCEditBox.h"

const std::string SavingUI::id = "saving";

SavingUI::SavingUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	//m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		m_saveButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_saveButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::saveCallback, this));
		m_saveButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, -widgetSize.height / 6));
		m_widget->addChild(m_saveButton);

		m_saveLabel = cocos2d::ui::Text::create("SAVE", "Thonburi", 21);
		m_saveLabel->setPosition(m_saveButton->getPosition() + cocos2d::Vec2(0, m_saveButton->getContentSize().height));
		m_widget->addChild(m_saveLabel);

		m_loadButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_loadButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::loadCallback, this));
		m_loadButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, widgetSize.height / 6));
		m_widget->addChild(m_loadButton);

		m_loadLabel = cocos2d::ui::Text::create("LOAD", "Thonburi", 21);
		m_loadLabel->setPosition(m_loadButton->getPosition() + cocos2d::Vec2(0, m_loadButton->getContentSize().height));
		m_widget->addChild(m_loadLabel);
	
		m_gemLabel = cocos2d::ui::Text::create("? GEM", "Thonburi", 21);
		m_gemLabel->setPosition(cocos2d::Vec2(0, widgetSize.height / 6));
		m_widget->addChild(m_gemLabel);

		m_goldLabel = cocos2d::ui::Text::create("? GOLD", "Thonburi", 21);
		m_goldLabel->setPosition(cocos2d::Vec2(0, -widgetSize.height / 6));
		m_widget->addChild(m_goldLabel);
	
		m_gemrandButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
		m_gemrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::gemrandCallback, this));
		m_gemrandButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, widgetSize.height / 6));
		m_widget->addChild(m_gemrandButton);

		m_goldrandButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
		m_goldrandButton->addTouchEventListener(CC_CALLBACK_2(SavingUI::goldrandCallback, this));
		m_goldrandButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, -widgetSize.height / 6));
		m_widget->addChild(m_goldrandButton);

		cocos2d::extension::EditBox* editor = cocos2d::extension::EditBox::create(cocos2d::Size(widgetSize.width / 2, 40), cocos2d::extension::Scale9Sprite::create("EditBox.png"));
		m_widget->addChild(editor);

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
		m_gemLabel->setText(WkCocos::ToolBox::itoa(g_gameLogic->getPlayer().m_gem.get<int>()) + " GEM");
	}
	if (m_goldLabel->isVisible())
	{
		m_goldLabel->setText(WkCocos::ToolBox::itoa(g_gameLogic->getPlayer().m_gold.get<int>()) + " GOLD");
	}

}

void SavingUI::saveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("SAVE BUTTON CLICKED");

		g_gameLogic->getPlayer().saveData();

	}
}


void SavingUI::loadCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("LOAD BUTTON CLICKED");

		g_gameLogic->getPlayer().loadData();

	}
}

void SavingUI::goldrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GOLD RAND BUTTON CLICKED");

		g_gameLogic->getPlayer().m_gold.set<int>( rand() % 10000);

	}
}


void SavingUI::gemrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("GEM RAND BUTTON CLICKED");

		g_gameLogic->getPlayer().m_gem.set<int>(rand() % 100);

	}
}


