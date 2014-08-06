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
				//This key is in clear in code, and store in public repository, because this is not a real published app.
				storeParams->setObject(cocos2d::__String::create("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArzKncfn7sGNNOg0p5Oz8zItUmaALQ7kivazs3WmdGXo9GVqzY6u3o2DylA9YuCCDeqLKklhhzTJbsd59cNFTQh/DN3rFA/7xn5AVb9Q6SopMYPHyqW0QGPS5cgXppGJ5I6tY6epBSuShwBXPmgAajbPF+/Viig11PDy4/iaqAWvvXG7TCwhEy2OklFH/VDwfES+rdH3nEd15SZ5xT4CstrnO+Dqcsi3iGa72Si5yWgzdCjzv0tHVR+WB5RwU1iYd4nl+qMF7GKlQVIE/AcTuOOh8eKOFQ2NP6eB42I1t3Sz8iXBo27C+D5cYMRqP2Gsy+8GVS8YgnO2BBSynXm6o7QIDAQAB"), "androidPublicKey");

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