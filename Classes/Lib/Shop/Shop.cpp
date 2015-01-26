#include "WkCocos/Shop/Shop.h"

using cocos2d::Ref;

namespace WkCocos
{

	namespace Shop
	{

		Shop::Shop(std::string googlePlayLicenseKey, std::string encryptLocalSecretKey, std::unique_ptr<Assets> a_shop_assets)
			: assets(std::move(a_shop_assets))
			, handler(new ShopEventHandler())
			, inventory(new Inventory())
		{
			cocos2d::__Dictionary *commonParams = cocos2d::__Dictionary::create();
			commonParams->setObject(cocos2d::__String::create(encryptLocalSecretKey), "customSecret");
			soomla::CCServiceManager::getInstance()->setCommonParams(commonParams);

			cocos2d::__Dictionary *storeParams = cocos2d::__Dictionary::create();
			storeParams->setObject(cocos2d::__String::create(googlePlayLicenseKey), "androidPublicKey");

			std::ostringstream msg;
			msg << "subscribing to Shop::MarketItemRefreshed event" << std::endl;
        	soomla::CCStoreUtils::logDebug(TAG, msg.str().c_str());

			//HACK to fix soomla refresh prices
			events()->subscribe<Shop::MarketItemRefreshed>(*this);

            soomla::CCStoreEventDispatcher::getInstance()->addEventHandler(handler.get());
			soomla::CCStoreService::initShared(assets->assets.get(), storeParams);

		}

        void Shop::receive(const WkCocos::Shop::Shop::MarketItemRefreshed& mir)
        {
            //we cannot dynamic cast on void*
            soomla::CCStoreUtils::logDebug(TAG, "[SOOMLA MARKET onMarketItemRefreshed receiver]");
            soomla::CCMarketItem* mi = mir.m_marketItem;
            if (assets && mi)
            {
                std::string productID = mi->getProductId()->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
                std::string marketPrice = "test";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string marketPrice = mi->getMarketPrice()->getCString();
#endif

                std::ostringstream logmsg;
                logmsg << "[SOOMLA MARKET onMarketItemRefreshed Listener] product:[" << productID << "] market price: [" << marketPrice << std::endl;
                soomla::CCStoreUtils::logDebug(TAG, logmsg.str().c_str());

                if (assets->marketPrices.end() == assets->marketPrices.find(productID))
                {
                    assets->marketPrices.insert(make_pair(productID, marketPrice));
                }
                else
                {
                    assets->marketPrices.at(productID) = marketPrice;
                }
            }
        }

		void Shop::activate()
		{


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//buggy ?
			//soomla::CCSoomlaStore::getInstance()->startIabServiceInBg();
#endif
		}

		void Shop::deactivate()
		{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//buggy ?
			//soomla::CCSoomlaStore::getInstance()->stopIabServiceInBg();
#endif

		}

		Shop::~Shop()
		{

		}


	}//namespace Shop
}//namespace WkCocos
