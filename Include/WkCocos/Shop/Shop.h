#ifndef __WKCOCOS_SHOP_SHOP_H__
#define __WKCOCOS_SHOP_SHOP_H__

#include "entityx/entityx.h"

#include "Cocos2dxStore.h"
#include "CCServiceManager.h"

#include "WkCocos/Shop/Inventory.h"
#include "WkCocos/Shop/Assets.h"
#include "WkCocos/Shop/SOOMLA/ShopEventHandler.h"

namespace WkCocos
{
	namespace Shop
	{
		class Shop
		{
		public:

			//It is not advised to have the googlePlayLicenseKey in clear in the code.
			//shopAsset is a unique pointer to show the takeover in ownership.
			Shop(std::string googlePlayLicenseKey, std::string encryptLocalSecretKey, std::unique_ptr<Assets> shopAssets);
			
			//activating the shop
			void activate();

			void deactivate();

			~Shop();

			entityx::ptr<entityx::EventManager> getEventManager()
			{
				return handler->m_event_manager;
			}

			std::shared_ptr<Inventory> getInventory()
			{
				return inventory;
			}

			Assets* getAssets()
			{
				return assets.get();
			}
			
			//Event that shop can emit
			struct BillingNotSupported : public entityx::Event<BillingNotSupported>{
			};

			struct BillingSupported : public entityx::Event<BillingSupported>{
			};

			struct CurrencyBalanceChanged : public entityx::Event<CurrencyBalanceChanged>{
				CurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded)
				: m_virtualCurrency(virtualCurrency->getName()->getCString(), virtualCurrency->getDescription()->getCString(), virtualCurrency->getItemId()->getCString())
				, m_balance(balance)
				, m_amountAdded(amountAdded)
				{}
				
				Assets::VirtualCurrency m_virtualCurrency;
				int m_balance;
				int m_amountAdded;
			};

			struct ItemPurchased : public entityx::Event<ItemPurchased>{
				ItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}
				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem *m_purchasableVirtualItem;
			};

			struct ItemPurchaseStarted : public entityx::Event<ItemPurchaseStarted>{
				ItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
			};

			struct MarketPurchaseCancelled : public entityx::Event<MarketPurchaseCancelled>{
				MarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
			};

			struct MarketPurchase : public entityx::Event<MarketPurchase>{
				MarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				, m_token(token->getCString())
				, m_payload(payload->getCString())
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
				std::string m_token;
				std::string m_payload;
			};

			struct MarketPurchaseStarted : public entityx::Event<MarketPurchaseStarted>{
				MarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
			};

			struct MarketPurchaseVerification : public entityx::Event<MarketPurchaseVerification>{
				MarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
			};

			struct RestoreTransactionsStarted : public entityx::Event<RestoreTransactionsStarted>{
			};

			struct RestoreTransactionsFinished : public entityx::Event<RestoreTransactionsFinished>{
				RestoreTransactionsFinished(bool success)
				: m_success(success)
				{}
				bool m_success;
			};

			struct UnexpectedErrorInStore : public entityx::Event<UnexpectedErrorInStore>{
			};

			struct StoreControllerInitialized : public entityx::Event<StoreControllerInitialized>{
			};

			struct MarketItemsRefreshed : public entityx::Event<MarketItemsRefreshed>{
			};

			struct MarketItemRefreshed : public entityx::Event<MarketItemRefreshed>{
				MarketItemRefreshed(soomla::CCMarketItem *mi)
				:m_marketItem(mi)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCMarketItem * m_marketItem;
			};
			
			struct MarketItemsRefreshStarted : public entityx::Event<MarketItemsRefreshStarted>{
			};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			struct MarketRefund : public entityx::Event<MarketRefund>{
				MarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem)
				: m_purchasableVirtualItem(purchasableVirtualItem)
				{}

				//TODO : use standard ( not soomla ) struct here for this item
				soomla::CCPurchasableVirtualItem * m_purchasableVirtualItem;
			};
			struct IabServiceStarted : public entityx::Event<IabServiceStarted>{
			};
			
			struct IabServiceStopped : public entityx::Event<IabServiceStopped>{
			};
#endif

		protected:
			std::unique_ptr<ShopEventHandler> handler;
			std::unique_ptr<Assets> assets;
			//shared pointer to be also owned by player...
			std::shared_ptr<Inventory> inventory;

		};

	}//namespace Shop
}// namespace WkCocos

#endif // __WKCOCOS_SHOP_SHOP_H__