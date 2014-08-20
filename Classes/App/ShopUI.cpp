#include "WkCocosApp/ShopUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

const std::string ShopUI::id = "shop";

//to not declare whole cocos namespace but sthill have cocos macros work...
using cocos2d::Ref;

ShopUI::ShopUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height * 0.67));
	cocos2d::Size widgetSize = m_widget->getContentSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	//building test UI based on ShopAssets content.
	std::vector<WkCocos::Shop::ShopAssets::VirtualCurrency> curncy = GameLogic::Instance().getShopAssets().getCurrenciesSTD();
	std::vector<WkCocos::Shop::ShopAssets::VirtualCurrencyPack> curncyPacks = GameLogic::Instance().getShopAssets().getCurrencyPacksSTD();
	
	for (auto c = curncy.begin(); c != curncy.end(); ++c)
	{
		//filtering which pack belong to this currency
		std::vector<WkCocos::Shop::ShopAssets::VirtualCurrencyPack> curncyPacksfiltered;
		std::copy_if(curncyPacks.begin(), curncyPacks.end(), std::back_inserter(curncyPacksfiltered), [c](WkCocos::Shop::ShopAssets::VirtualCurrencyPack vcp) -> bool { return vcp.currencyID == c->itemid; });

		//creating label for currency
		cocos2d::ui::Text* curtxtui = cocos2d::ui::Text::create(c->name, "Arial", 21);
		curtxtui->setPosition(cocos2d::Vec2(
			widgetSize.width / (curncyPacksfiltered.size() + 2),
			widgetSize.height * ((curncy.end() - c) + 1) / (curncy.size() + 2))
			);
		m_curLabel.push_back(curtxtui);
		m_widget->addChild(curtxtui);

		//parsing currency packs
		for (auto p = curncyPacksfiltered.begin(); p != curncyPacksfiltered.end(); ++p)
		{
			//creating button
			cocos2d::ui::Button * butt = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
			butt->addTouchEventListener(std::bind(&ShopUI::buyCallback, this, *p, std::placeholders::_1,std::placeholders::_2));
			butt->setPosition(cocos2d::Vec2(
				(widgetSize.width - butt->getContentSize().width) * ((curncyPacksfiltered.end() - p) + 1) / (curncyPacksfiltered.size() + 2),
				curtxtui->getPosition().y
				)
			);
			m_buyCurButton.push_back(butt);
			m_widget->addChild(butt);

			//creating label
			cocos2d::ui::Text* curptxtui = cocos2d::ui::Text::create(p->name, "Arial", 21);
			curptxtui->setPosition(cocos2d::Vec2(butt->getPosition() + cocos2d::Vec2(0, butt->getContentSize().height))
			);
			m_curLabel.push_back(curptxtui);
			m_widget->addChild(curptxtui);

			//creating price text
			cocos2d::ui::Text* curppriceui = cocos2d::ui::Text::create(WkCocos::ToolBox::itoa(p->price), "Arial", 21);
			curppriceui->setPosition(cocos2d::Vec2(butt->getPosition() - cocos2d::Vec2(0, butt->getContentSize().height))
				);
			m_curLabel.push_back(curppriceui);
			m_widget->addChild(curppriceui);
		}

	}

	//building global shop UI

	//button to test getting information from googleplay ( update in prices )
	m_refreshButton = cocos2d::ui::Button::create("RefreshNormal.png", "RefreshSelected.png");
	m_refreshButton->addTouchEventListener(CC_CALLBACK_2(ShopUI::refreshCallback, this));
	m_refreshButton->setPosition(cocos2d::Vec2(
		(widgetSize.width - m_refreshButton->getContentSize().width) * 2 / 4,
		(widgetSize.height - m_refreshButton->getContentSize().height) / (curncy.size() + 2)
		));
	m_widget->addChild(m_refreshButton);
	m_refreshLabel = cocos2d::ui::Text::create("REFRESH", "Arial", 21);
	m_refreshLabel->setPosition(m_refreshButton->getPosition() + cocos2d::Vec2(0, m_refreshButton->getContentSize().height));
	m_widget->addChild(m_refreshLabel);

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

void ShopUI::buyCallback( WkCocos::Shop::ShopAssets::VirtualCurrencyPack vcp, cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("BUY ITEM %s BUTTON CLICKED", vcp.productID.c_str());

		//test buy

		soomla::CCError *soomlaError = NULL;
		soomla::CCStoreInventory::sharedStoreInventory()->buyItem(vcp.itemid.c_str(), &soomlaError);
		
		if (soomlaError) {
			soomla::CCStoreUtils::logException("ShopUI::buyCallback", soomlaError);
			return;
		}
	}
}


void ShopUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("REFRESH BUTTON CLICKED");

		//test buy

		soomla::CCError *soomlaError = NULL;
		soomla::CCSoomlaStore::getInstance()->refreshInventory();
		if (soomlaError) {
			soomla::CCStoreUtils::logException("ShopUI::refreshCallback", soomlaError);
			return;
		}
	}
}


