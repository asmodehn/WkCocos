#ifndef __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
#define __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__

#include "entityx/entityx.h"

#include "Soomla/CCStoreAssets.h"
#include "cocos2d.h"

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

#define GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID "android.test.refunded"
#define GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID "android.test.canceled"
#define GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID "android.test.purchased"
#define GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID "android.test.item_unavailable"

namespace WkCocos
{
	namespace Shop
	{
		namespace SOOMLA
		{

			class ShopAssets : public soomla::CCStoreAssets {
			public:

				struct VirtualGood{
					std::string name;
					std::string description;
					std::string idemID;
					std::string vCurrencyID;
					unsigned int price;
				};

				struct VirtualCategory{
					std::string categoryID;
					std::vector<std::string> goodsID;
				};

				struct NonConsumable{
					std::string name;
					std::string description;
					std::string itemID;
					std::string productID;
					double price;
				};

				ShopAssets();

				virtual ~ShopAssets();

				static ShopAssets *create();
				bool init();

				virtual int getVersion();


				bool addVirtualCurrency(soomla::CCVirtualCurrency * vc);
				bool addVirtualCurrencyPack(soomla::CCVirtualCurrencyPack * vcp);

				//uses structure to provide differnet interface using structure.
				bool addVirtualGood(std::string name, std::string description, std::string idemID, std::string vCurrencyID, unsigned int price);
				bool buildVirtualCategory(std::string categoryID, std::vector<std::string> goodsID);
				bool addNonConsumable(std::string name, std::string description, std::string itemID, std::string productID, double price);

				//WARNING : Do noot use this directly in game. Use CCStoreInfo instead.
				virtual cocos2d::CCArray *getCurrencies() override;
				virtual cocos2d::CCArray *getCurrencyPacks() override;
				virtual cocos2d::CCArray *getGoods() override;
				virtual cocos2d::CCArray *getCategories() override;
				virtual cocos2d::CCArray *getNonConsumableItems() override;

			private:

				cocos2d::Array *mCurrencies;
				cocos2d::Array *mGoods;
				cocos2d::Array *mCurrencyPacks;
				cocos2d::Array *mCategories;
				cocos2d::Array *mNonConsumableItems;
			};

		} //namespace SOOMLA
	} //namespace Shop
}//namespace WkCocos

#endif //__WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
