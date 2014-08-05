#ifndef __WKCOCOS_SHOP_SHOP_H__
#define __WKCOCOS_SHOP_SHOP_H__

#include "entityx/entityx.h"

#include "Cocos2dxStore.h"
#include "CCServiceManager.h"

#include "WkCocos/Shop/ShopEventHandler.h"
#include "WkCocos/Shop/MuffinRushAssets.h"

namespace WkCocos
{
	namespace Shop
	{

		class Shop
		{
		public:

			Shop()
			{

				handler = new ShopEventHandler();

				cocos2d::__Dictionary *commonParams = cocos2d::__Dictionary::create();
				commonParams->setObject(cocos2d::__String::create("ExampleCustomSecret"), "customSecret");
				soomla::CCServiceManager::getInstance()->setCommonParams(commonParams);

				cocos2d::__Dictionary *storeParams = cocos2d::__Dictionary::create();
				storeParams->setObject(cocos2d::__String::create("ExamplePublicKey"), "androidPublicKey");

				MuffinRushAssets *assets = MuffinRushAssets::create();

				soomla::CCStoreService::initShared(assets, storeParams);

				soomla::CCStoreEventDispatcher::getInstance()->addEventHandler(handler);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				soomla::CCSoomlaStore::getInstance()->startIabServiceInBg();
#endif

			}

			~Shop()
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				soomla::CCSoomlaStore::getInstance()->stopIabServiceInBg();
#endif

			}

			ShopEventHandler* handler;
		};

	}//namespace Shop
}// namespace WkCocos

#endif // __WKCOCOS_SHOP_SHOP_H__