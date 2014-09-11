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

			soomla::CCStoreService::initShared(assets->assets.get(), storeParams);

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