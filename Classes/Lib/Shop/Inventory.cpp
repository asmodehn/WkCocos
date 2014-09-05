#include "WkCocos/Shop/Inventory.h"

#include "Soomla/CCStoreInventory.h"
#include "Soomla/CCSoomlaStore.h"

using cocos2d::Ref;

namespace WkCocos
{

	namespace Shop
	{

		Inventory::Inventory()
		{
		}

		Inventory::~Inventory()
		{

		}

		int Inventory::getBalance(std::string itemid)
		{
			soomla::CCError * err = NULL;
			int bal = soomla::CCStoreInventory::sharedStoreInventory()->getItemBalance(itemid.c_str(), &err);
			if (!err)
			{
				return bal;
			}
			else
			{
				return 0;
			}
		}

		void Inventory::refresh()
		{
			soomla::CCError *soomlaError = NULL;
			soomla::CCSoomlaStore::getInstance()->refreshInventory();
			if (soomlaError) {
				CCLOGERROR("Inventory::refresh", soomlaError);
				return;
			}
		}

		void Inventory::buy(std::string itemid)
		{
			soomla::CCError *soomlaError = NULL;
			soomla::CCStoreInventory::sharedStoreInventory()->buyItem(itemid.c_str(), &soomlaError);
			if (soomlaError) {
				CCLOGERROR("Inventory::buy", soomlaError);
				return;
			}
		}

	}//namespace Shop
}//namespace WkCocos