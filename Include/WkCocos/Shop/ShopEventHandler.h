#ifndef __WKCOCOS_SHOP_SHOPEVENTHANDLER_H__
#define __WKCOCOS_SHOP_SHOPEVENTHANDLER_H__

#include <string>
#include "Cocos2dxStore.h"


#define EVENT_ON_CURRENCY_BALANCE_CHANGED "onCurrencyBalanceChanged"
#define EVENT_ON_GOOD_BALANCE_CHANGED "onGoodBalanceChanged"
#define EVENT_ON_GOOD_EQUIPPED "onGoodEquipped"
#define EVENT_ON_GOOD_UNEQUIPPED "onGoodUnEquipped"
#define EVENT_ON_GOOD_UPGRADE "onGoodUpgrade"

using namespace std;

class ShopEventHandler : public soomla::CCStoreEventHandler {
public:

	virtual void onBillingNotSupported();

	virtual void onBillingSupported();

	virtual void onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded);

	virtual void onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded);

	virtual void onGoodEquipped(soomla::CCEquippableVG *equippableVG);

	virtual void onGoodUnEquipped(soomla::CCEquippableVG *equippableVG);

	virtual void onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG);

	virtual void onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

	virtual void onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

	virtual void onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

	virtual void onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload);

	virtual void onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

	virtual void onMarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

	virtual void onRestoreTransactionsStarted();

	virtual void onRestoreTransactionsFinished(bool success);

	virtual void onUnexpectedErrorInStore();

	virtual void onStoreControllerInitialized();

	virtual void onMarketItemsRefreshed();

	virtual void onMarketItemsRefreshStarted();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);
	virtual void onIabServiceStarted();
	virtual void onIabServiceStopped();
#endif
};

#endif //__WKCOCOS_SHOP_SHOPEVENTHANDLER_H__
