#ifndef __WKCOCOS_SHOP_ASSETS_H__
#define __WKCOCOS_SHOP_ASSETS_H__

//soomla implementation
#include "WkCocos/Shop/SOOMLA/ShopAssets.h"

namespace WkCocos
{
	namespace Shop
	{
		class Assets
		{
		public:

			//It is not advised to have the googlePlayLicenseKey in clear in the code.
			Assets();

			struct VirtualCurrency{
				VirtualCurrency(std::string aname, std::string adesc, std::string aitemid)
				: name(aname), description(adesc), itemid(aitemid)
				{}
				std::string name;
				std::string description;
				std::string itemid;
			};

			struct VirtualCurrencyPack{
				VirtualCurrencyPack(std::string aname, std::string adesc, std::string aitemid, unsigned long acurrencyAmount, std::string acurrencyID, std::string aproductID, double aprice, std::string amarketPrice, std::string amarketTitle, std::string amarketDescription)
				: name(aname), description(adesc), itemid(aitemid), currencyAmount(acurrencyAmount), currencyID(acurrencyID), productID(aproductID), price(aprice), marketPrice(amarketPrice), marketTitle(amarketTitle), marketDescription(amarketDescription)
				{}
				std::string name;
				std::string description;
				std::string itemid;
				unsigned long currencyAmount;
				std::string currencyID;
				std::string productID;
				double price;
				std::string marketPrice;
				std::string marketTitle;
				std::string marketDescription;
			};

			/**
			* Adding virtual currency
			* @param name : The name of the currency
			* @param description : The description of the currency
			* @param itemid : The local Id to identify the currency in this code project.
			*/
			inline bool addVirtualCurrency(std::string name, std::string description, std::string itemid)
			{
				return addVirtualCurrency(VirtualCurrency(name, description, itemid));
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
				//creating virutal currency pack without information from the market
				return addVirtualCurrencyPack(VirtualCurrencyPack(name, description, itemid, currencyAmount, currencyID, productID, price, "", "", ""));
			}
			bool addVirtualCurrencyPack(VirtualCurrencyPack vcp);

			//retrieving list of currencies and packs
			virtual std::vector<VirtualCurrency> getCurrencies();
			virtual std::vector<VirtualCurrencyPack> getCurrencyPacks();

			~Assets();

		protected:
			friend class Shop;
			std::shared_ptr<SOOMLA::ShopAssets> assets;

			//SOOMLA BUG workaround
			std::map<std::string, std::string> marketPrices;
		};

	}//namespace Shop
}// namespace WkCocos

#endif // __WKCOCOS_SHOP_SHOP_H__