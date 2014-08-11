#include "WkCocosApp/ShopUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

const std::string ShopUI::id = "shop";

ShopUI::ShopUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	m_buyButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
	m_buyButton->addTouchEventListener(CC_CALLBACK_2(ShopUI::buyCallback, this));
	m_buyButton->setPosition(cocos2d::Vec2(
		(visibleSize.width - m_buyButton->getContentSize().width) / 4,
		(visibleSize.height - m_buyButton->getContentSize().height) / 3
	));
	m_widget->addChild(m_buyButton);
	m_buyLabel = cocos2d::ui::Text::create("BUY", "Arial", 21);
	m_buyLabel->setPosition(m_buyButton->getPosition() + cocos2d::Vec2(0, m_buyButton->getContentSize().height));
	m_widget->addChild(m_buyLabel);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

ShopUI::~ShopUI()
{}

void ShopUI::update(float delta)
{
}

void ShopUI::buyCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("BUY BUTTON CLICKED");

		//test buy

		soomla::CCError *soomlaError = NULL;
		soomla::CCStoreInventory::sharedStoreInventory()->buyItem("muffins_10", &soomlaError);
		if (soomlaError) {
			soomla::CCStoreUtils::logException("StoreBScene::onBuy", soomlaError);
			return;
		}
	}
}



