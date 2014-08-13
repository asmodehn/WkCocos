//
// Created by Fedor Shubin on 5/24/13.
//


#include "WkCocos/Shop/ShopAssets.h"

#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualGood.h"
#include "CCSingleUseVG.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "CCVirtualCategory.h"
#include "CCNonConsumableItem.h"
#include "CCUpgradeVG.h"
#include "CCSingleUsePackVG.h"
#include "CCEquippableVG.h"

#define MUFFIN_CURRENCY_ITEM_ID "currency_muffin"
#define TENMUFF_PACK_PRODUCT_ID "android.test.refunded"
#define FIFTYMUFF_PACK_PRODUCT_ID "android.test.canceled"
#define FOURHUNDMUFF_PACK_PRODUCT_ID "android.test.purchased"
#define THOUSANDMUFF_PACK_PRODUCT_ID "android.test.item_unavailable"
#define NO_ADDS_NONCONS_PRODUCT_ID "gem_5"

#define MUFFINCAKE_ITEM_ID "fruit_cake"
#define PAVLOVA_ITEM_ID "pavlova"
#define CHOCLATECAKE_ITEM_ID "chocolate_cake"
#define CREAMCUP_ITEM_ID "cream_cup"

namespace WkCocos
{

	namespace Shop
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

		bool ShopAssets::addVirtualCurrency(struct VirtualCurrency vc)
		{
			/** Virtual Currencies **/
			soomla::CCVirtualCurrency *vCurrency = soomla::CCVirtualCurrency::create(
				cocos2d::CCString::create(vc.name),
				cocos2d::CCString::create(vc.description),
				cocos2d::CCString::create(vc.itemid)
				);

			mCurrencies->addObject(vCurrency);

			return true;
		}

		bool ShopAssets::addVirtualCurrencyPack(struct VirtualCurrencyPack vcp)
		{
			/** Virtual Currency Packs **/
			soomla::CCVirtualCurrencyPack *vCurrency = soomla::CCVirtualCurrencyPack::create(
				cocos2d::CCString::create(vcp.name),
				cocos2d::CCString::create(vcp.description),
				cocos2d::CCString::create(vcp.itemid),
				cocos2d::CCInteger::create(vcp.currencyAmount),
				cocos2d::CCString::create(vcp.currencyID),
				soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(vcp.productID), cocos2d::CCDouble::create(vcp.price))
				);

			mCurrencyPacks->addObject(vCurrency);

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

			soomla::CCVirtualCategory *vCat = soomla::CCVirtualCategory::create(cocos2d::CCString::create(categoryID),goods);

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
						cocos2d::CCInteger::create(soomla::CCMarketItem::NONCONSUMABLE), cocos2d::CCDouble::create(price)
					)
				)
			);

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


		std::vector<ShopAssets::VirtualCurrency> ShopAssets::getCurrenciesSTD()
		{
			std::vector<VirtualCurrency> stdVCur;

			//we need to rebuild the vector from the CCArray in case soomla changes things ( retrieve prices, etc.)
			cocos2d::CCObject* c;
			CCARRAY_FOREACH(mCurrencies, c)
			{
				soomla::CCVirtualCurrency* vc = dynamic_cast<soomla::CCVirtualCurrency*>(c);
				cocos2d::Dictionary* vcd = vc->toDictionary();

				std::string curname(vc->getName()->getCString());
				std::string curdesc(vc->getDescription()->getCString());
				std::string curid(vc->getItemId()->getCString());

				stdVCur.push_back(VirtualCurrency( curname, curdesc, curid));

			}
			return stdVCur;
		}


		cocos2d::CCArray *ShopAssets::getCurrencyPacks() {
			return mCurrencyPacks;
		}

		std::vector<ShopAssets::VirtualCurrencyPack> ShopAssets::getCurrencyPacksSTD()
		{
			std::vector<VirtualCurrencyPack> stdVCPack;

			//we need to rebuild the vector from the CCArray in case soomla changes things ( retrieve prices, etc.)
			cocos2d::CCObject* c;
			CCARRAY_FOREACH(mCurrencyPacks, c)
			{
				soomla::CCVirtualCurrencyPack* vc = dynamic_cast<soomla::CCVirtualCurrencyPack*>(c);
				cocos2d::Dictionary* vcd = vc->toDictionary();
				
				std::string curpname(vc->getName()->getCString());
				std::string curpdesc(vc->getDescription()->getCString());
				std::string curpid(vc->getItemId()->getCString());

				
				unsigned int curpcurrencyamount(vc->getCurrencyAmount()->getValue());
				std::string curpcurrencyid(vc->getCurrencyItemId()->getCString());

				soomla::CCPurchaseWithMarket* pwm = dynamic_cast<soomla::CCPurchaseWithMarket*>(vc->getPurchaseType());

				std::string curproductid(pwm->getMarketItem()->getProductId()->getCString());
				unsigned int curconsume(pwm->getMarketItem()->getConsumable()->getValue());
				double curprice(pwm->getMarketItem()->getPrice()->getValue());

				stdVCPack.push_back(VirtualCurrencyPack(curpname, curpdesc, curpid, curpcurrencyamount, curpcurrencyid, curproductid, curprice));

			}
			return stdVCPack;
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


	} //namespace Shop
} // namespace WkCocos
