//
// Created by Fedor Shubin on 5/24/13.
//


#include "WkCocos/Shop/SOOMLA/ShopAssets.h"
//to grab event when market items are refreshed, and maintain the market price list
#include "WkCocos/Shop/SOOMLA/ShopEventHandler.h"

#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualGood.h"
#include "CCSingleUseVG.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "CCMarketItem.h"
#include "CCVirtualCategory.h"
#include "CCNonConsumableItem.h"
#include "CCUpgradeVG.h"
#include "CCSingleUsePackVG.h"
#include "CCEquippableVG.h"

#define TAG "ShopAssets >>>"

namespace WkCocos
{

	namespace Shop
	{
		namespace SOOMLA
		{

			ShopAssets::ShopAssets() :
				mCurrencies(NULL),
				mGoods(NULL),
				mCurrencyPacks(NULL),
				mCategories(NULL),
				mNonConsumableItems(NULL)
			{
				mCurrencies = cocos2d::CCArray::create();
				mCurrencies->retain();
				mCurrencyPacks = cocos2d::CCArray::create();
				mCurrencyPacks->retain();
				mGoods = cocos2d::CCArray::create();
				mGoods->retain();
				mCategories = cocos2d::CCArray::create();
				mCategories->retain();
				mNonConsumableItems = cocos2d::CCArray::create();
				mNonConsumableItems->retain();
				
			}

			bool ShopAssets::addVirtualCurrency(soomla::CCVirtualCurrency * vc)
			{
				mCurrencies->addObject(vc);

				return true;
			}

			bool ShopAssets::addVirtualCurrencyPack(soomla::CCVirtualCurrencyPack * vcp)
			{
				mCurrencyPacks->addObject(vcp);
				return true;
			}


			bool ShopAssets::addVirtualGood(std::string name, std::string description, std::string idemID, std::string vCurrencyID, unsigned int price)
			{
				/** Virtual Goods **/

				soomla::CCVirtualGood *virtualGood = soomla::CCSingleUseVG::create(
					cocos2d::CCString::create(name),                                       // name
					cocos2d::CCString::create(description), // description
					cocos2d::CCString::create(idemID),                                       // item id
					soomla::CCPurchaseWithVirtualItem::create(
					cocos2d::CCString::create(vCurrencyID), cocos2d::CCInteger::create(price)
					) // the way this virtual good is purchased
					);

				mGoods->addObject(virtualGood);

				return true;
			}

			bool ShopAssets::buildVirtualCategory(std::string categoryID, std::vector<std::string> goodsID)
			{
				/** Virtual Categories **/

				cocos2d::CCArray* goods = cocos2d::CCArray::create();

				for (std::string good : goodsID)
				{
					goods->addObject(cocos2d::CCString::create(good));
				}

				soomla::CCVirtualCategory *vCat = soomla::CCVirtualCategory::create(cocos2d::CCString::create(categoryID), goods);

				mCategories->addObject(vCat);

				return true;
			}

			bool ShopAssets::addNonConsumable(std::string name, std::string description, std::string itemID, std::string productID, double price)
			{

				/** Google MANAGED /?/ NonConsumable Items **/

				soomla::CCNonConsumableItem *nonConsumable = soomla::CCNonConsumableItem::create(
					cocos2d::CCString::create(name),
					cocos2d::CCString::create(description),
					cocos2d::CCString::create(itemID),
					soomla::CCPurchaseWithMarket::createWithMarketItem(
						soomla::CCMarketItem::create(
							cocos2d::CCString::create(productID),
							cocos2d::CCInteger::create(soomla::CCMarketItem::NONCONSUMABLE),
							cocos2d::CCDouble::create(price)
						)
					));

				mNonConsumableItems->addObject(nonConsumable);

				return true;
			}

			ShopAssets::~ShopAssets() {
				CC_SAFE_RELEASE(mCurrencies);
				CC_SAFE_RELEASE(mGoods);
				CC_SAFE_RELEASE(mCurrencyPacks);
				CC_SAFE_RELEASE(mCategories);
				CC_SAFE_RELEASE(mNonConsumableItems);
			}

			int ShopAssets::getVersion() {
				//increase version erase DB content.
				return 0;
			}

			cocos2d::CCArray *ShopAssets::getCurrencies() {
				return mCurrencies;
			}

			cocos2d::CCArray *ShopAssets::getCurrencyPacks() {
				return mCurrencyPacks;
			}

			cocos2d::CCArray *ShopAssets::getGoods() {
				return mGoods;
			}


			cocos2d::CCArray *ShopAssets::getCategories() {
				return mCategories;
			}

			cocos2d::CCArray *ShopAssets::getNonConsumableItems() {
				return mNonConsumableItems;
			}

		} //namespace SOOMLA
	} //namespace Shop
} // namespace WkCocos
