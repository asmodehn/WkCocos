#ifndef __WKCOCOS_SHOP_SHOP_H__
#define __WKCOCOS_SHOP_SHOP_H__

#include "entityx/entityx.h"

#include "Cocos2dxStore.h"
#include "CCServiceManager.h"

#include "WkCocos/Shop/ShopEventHandler.h"
#include "WkCocos/Shop/ShopAssets.h"

namespace WkCocos
{
	namespace Shop
	{

		class Shop
		{
		public:

			//It is not advised to have the googlePlayLicenseKey in clear in the code.
			Shop(std::string googlePlayLicenseKey, std::string encryptLocalSecretKey, std::shared_ptr<ShopAssets> shopAssets);

			void activate();

			void deactivate();

			~Shop();

		protected:
			std::unique_ptr<ShopEventHandler> handler;

			std::shared_ptr<ShopAssets> assets;

		};

	}//namespace Shop
}// namespace WkCocos

#endif // __WKCOCOS_SHOP_SHOP_H__