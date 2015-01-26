#include "WkCocos/Shop/Assets.h"
#include "WkCocos/Shop/SOOMLA/ShopAssets.h"
#include "WkCocos/Shop/SOOMLA/ShopEventHandler.h"
using cocos2d::Ref;

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "Soomla/domain/virtualGoods/CCVirtualGood.h"
#include "Soomla/domain/virtualGoods/CCSingleUseVG.h"
#include "Soomla/PurchaseTypes/CCPurchaseWithMarket.h"
#include "Soomla/PurchaseTypes/CCPurchaseWithVirtualItem.h"
#include "Soomla/domain/CCVirtualCategory.h"
#include "Soomla/domain/CCNonConsumableItem.h"
#include "Soomla/domain/virtualGoods/CCUpgradeVG.h"
#include "Soomla/domain/virtualGoods/CCSingleUsePackVG.h"
#include "Soomla/domain/virtualGoods/CCEquippableVG.h"
#include "Soomla/data/CCStoreInfo.h"

#define TAG "Assets"
namespace WkCocos
{

	namespace Shop
	{

		Assets::Assets()
			: assets(new SOOMLA::ShopAssets())
		{
		    std::ostringstream msg;
			msg << "creating Shop::Assets. Adding CustomEventListener for onMarketItemRefreshed onto " << cocos2d::Director::getInstance()->getEventDispatcher() << std::endl;
        	soomla::CCStoreUtils::logDebug(TAG, msg.str().c_str());

			//HACK to fix soomla refresh prices
			//CCSoomlaEventDispatcher::getInstance() ??
			cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_ON_MARKET_ITEM_REFRESHED, [=](cocos2d::EventCustom* ec){

				//we cannot dynamic cast on void*
				soomla::CCStoreUtils::logDebug(TAG, "[SOOMLA MARKET onMarketItemRefreshed Listener]");
				soomla::CCMarketItem* mi = static_cast<soomla::CCMarketItem*>(ec->getUserData());
				if (mi)
				{
					std::string productID = mi->getProductId()->getCString();
					std::string marketPrice = mi->getMarketPrice()->getCString();

					std::ostringstream logmsg;
					logmsg << "[SOOMLA MARKET onMarketItemRefreshed Listener] product:[" << productID << "] market price: [" << marketPrice << std::endl;
					soomla::CCStoreUtils::logDebug(TAG, logmsg.str().c_str());

					if (marketPrices.end() == marketPrices.find(productID))
					{
						marketPrices.insert(make_pair(productID, marketPrice));
					}
					else
					{
						marketPrices.at(productID) = marketPrice;
					}
				}
			});

		}

		Assets::~Assets()
		{

		}

		bool Assets::addVirtualCurrency(struct Assets::VirtualCurrency vc)
		{
			/** Virtual Currencies **/
			soomla::CCVirtualCurrency *vCurrency = soomla::CCVirtualCurrency::create(
				cocos2d::CCString::create(vc.name),
				cocos2d::CCString::create(vc.description),
				cocos2d::CCString::create(vc.itemid)
				);

			assets->addVirtualCurrency(vCurrency);

			return true;
		}

		bool Assets::addVirtualCurrencyPack(struct Assets::VirtualCurrencyPack vcp)
		{
			/** Virtual Currency Packs **/
			soomla::CCVirtualCurrencyPack *vCurrencyPack = soomla::CCVirtualCurrencyPack::create(
				cocos2d::CCString::create(vcp.name),
				cocos2d::CCString::create(vcp.description),
				cocos2d::CCString::create(vcp.itemid),
				cocos2d::CCInteger::create(vcp.currencyAmount),
				cocos2d::CCString::create(vcp.currencyID),
				soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(vcp.productID), cocos2d::CCDouble::create(vcp.price))
				);

			assets->addVirtualCurrencyPack(vCurrencyPack);

			return true;
		}

		std::vector<Assets::VirtualCurrency> Assets::getCurrencies()
		{
			std::vector<Assets::VirtualCurrency> stdVCur;

			//we need to build the vector from the CCArray.
			cocos2d::CCObject* c;
			cocos2d::__Array* currency_array = soomla::CCStoreInfo::sharedStoreInfo()->getVirtualCurrencies();
			CCARRAY_FOREACH(currency_array, c)
			{
				soomla::CCVirtualCurrency* vc = dynamic_cast<soomla::CCVirtualCurrency*>(c);
				cocos2d::Dictionary* vcd = vc->toDictionary();

				std::string curname(vc->getName()->getCString());
				std::string curdesc(vc->getDescription()->getCString());
				std::string curid(vc->getItemId()->getCString());

				stdVCur.push_back(VirtualCurrency(curname, curdesc, curid));

			}
			return stdVCur;
		}

		std::vector<Assets::VirtualCurrencyPack> Assets::getCurrencyPacks()
		{
			std::vector<Assets::VirtualCurrencyPack> stdVCPack;

			//we need to build the vector from the CCArray.
			cocos2d::CCObject* c;
			cocos2d::__Array* currencyPack_array = soomla::CCStoreInfo::sharedStoreInfo()->getVirtualCurrencyPacks();
			CCARRAY_FOREACH(currencyPack_array, c)
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

				cocos2d::CCString* marketPrice = pwm->getMarketItem()->getMarketPrice();
				std::string curMPrice;
				if (marketPrice) //will be null if no refresh of the store was possible yet
				{
					curMPrice = std::string(marketPrice->getCString());
				}
				else if (marketPrices.end() != marketPrices.find(curproductid)) //if it s null but we have it in the market price workaround list
				{
					curMPrice = marketPrices.at(curproductid);
				}

				cocos2d::CCString* marketTitle = pwm->getMarketItem()->getMarketTitle();
				std::string curMTitle;
				if (marketTitle)//will be null if no refresh of the store was possible yet
				{
					curMTitle = std::string(marketTitle->getCString());
				}

				cocos2d::CCString* marketDescription = pwm->getMarketItem()->getMarketDescription();
				std::string curMDescription;
				if (marketDescription)//will be null if no refresh of the store was possible yet
				{
					curMDescription = std::string(marketDescription->getCString());
				}

				stdVCPack.push_back(Assets::VirtualCurrencyPack(curpname, curpdesc, curpid, curpcurrencyamount, curpcurrencyid, curproductid, curprice, curMPrice, curMTitle, curMDescription));

			}
			return stdVCPack;
		}

	}//namespace Shop
}//namespace WkCocos
