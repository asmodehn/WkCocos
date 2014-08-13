#ifndef __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
#define __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__

#include "entityx/entityx.h"

#include "CCStoreAssets.h"
#include "cocos2d.h"

#define GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID "android.test.refunded"
#define GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID "android.test.canceled"
#define GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID "android.test.purchased"
#define GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID "android.test.item_unavailable"

namespace WkCocos
{
	namespace Shop
	{

		class ShopAssets : public soomla::CCStoreAssets {
		public:

			struct VirtualCurrency{
				VirtualCurrency(std::string aname, std::string adesc, std::string aitemid)
				: name(aname), description(adesc), itemid(aitemid)
				{}
				std::string name;
				std::string description;
				std::string itemid;
			};

			struct VirtualCurrencyPack{
				VirtualCurrencyPack(std::string aname, std::string adesc, std::string aitemid, unsigned long acurrencyAmount, std::string acurrencyID, std::string aproductID, double aprice)
				: name(aname), description(adesc), itemid(aitemid), currencyAmount(acurrencyAmount), currencyID(acurrencyID), productID(aproductID), price(aprice)
				{}
				std::string name;
				std::string description;
				std::string itemid;
				unsigned long currencyAmount;
				std::string currencyID;
				std::string productID;
				double price;
			};

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

			/**
			* Adding virtual currency
			* @param name : The name of the currency
			* @param description : The description of the currency
			* @param itemid : The local Id to identify the currency in this code project.
			*/
			inline bool addVirtualCurrency(std::string name, std::string description, std::string itemid)
			{
				return addVirtualCurrency(VirtualCurrency( name, description, itemid ));
			}
			bool addVirtualCurrency(VirtualCurrency vc);
			/**
			* Adding virtual currency pack
			* @param name : The name of the pack
			* @param description : The description of the pack
			* @param itemid : The local Id to identify the pack in this code project.
			* @param currencyAmount : the amount of currency increase when buying this pack.
			* @param currencyID : the currency to increase when buying this pack.
			* @param productID :The online store (googleplay/applestore) id for this pack.
			* @param price:The online store (googleplay/applestore) price for this pack.
			*/
			inline bool addVirtualCurrencyPack(std::string name, std::string description, std::string itemid, unsigned long currencyAmount, std::string currencyID, std::string productID, double price)
			{
				return addVirtualCurrencyPack(VirtualCurrencyPack( name, description, itemid, currencyAmount, currencyID, productID, price));
			}
			bool addVirtualCurrencyPack(VirtualCurrencyPack vcp);

			//TODO : uses structure to provide differnet interface using structure.
			bool addVirtualGood(std::string name, std::string description, std::string idemID, std::string vCurrencyID, unsigned int price);
			bool buildVirtualCategory(std::string categoryID, std::vector<std::string> goodsID);
			bool addNonConsumable(std::string name, std::string description, std::string itemID, std::string productID, double price);

			virtual cocos2d::CCArray *getCurrencies() override;
			virtual std::vector<ShopAssets::VirtualCurrency> getCurrenciesSTD();

			virtual cocos2d::CCArray *getCurrencyPacks() override;
			virtual std::vector<ShopAssets::VirtualCurrencyPack> getCurrencyPacksSTD();

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

	} //namespace Shop
}//namespace WkCocos

#endif //__WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
