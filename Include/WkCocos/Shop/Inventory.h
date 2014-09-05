#ifndef __WKCOCOS_SHOP_INVENTORY_H__
#define __WKCOCOS_SHOP_INVENTORY_H__

#include "entityx/entityx.h"

#include "Cocos2dxStore.h"
#include "CCServiceManager.h"

namespace WkCocos
{
	namespace Shop
	{
		class Inventory
		{
		public:

			//It is not advised to have the googlePlayLicenseKey in clear in the code.
			Inventory();

			//retrieving current balance
			int getBalance(std::string itemid);

			void refresh();

			void buy(std::string itemid);

			~Inventory();

			
		};

	}//namespace Shop
}// namespace WkCocos

#endif // __WKCOCOS_SHOP_INVENTORY_H__