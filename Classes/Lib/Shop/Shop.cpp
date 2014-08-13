#include "WkCocos/Shop/Shop.h"

namespace WkCocos
{

	namespace Shop
	{

		Shop::Shop(std::string googlePlayLicenseKey, std::string encryptLocalSecretKey, std::shared_ptr<ShopAssets> a_shop_assets)
		{
			handler.reset(new ShopEventHandler());

			cocos2d::__Dictionary *commonParams = cocos2d::__Dictionary::create();
			commonParams->setObject(cocos2d::__String::create(encryptLocalSecretKey), "customSecret");
			soomla::CCServiceManager::getInstance()->setCommonParams(commonParams);

			cocos2d::__Dictionary *storeParams = cocos2d::__Dictionary::create();
			storeParams->setObject(cocos2d::__String::create(googlePlayLicenseKey), "androidPublicKey");

			assets = a_shop_assets;

			soomla::CCStoreService::initShared(assets.get(), storeParams);

			soomla::CCStoreEventDispatcher::getInstance()->addEventHandler(handler.get());

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