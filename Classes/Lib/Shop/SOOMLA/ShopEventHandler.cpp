#include "WkCocos/Shop/SOOMLA/ShopEventHandler.h"
#include "cocos2d.h"

//to get the definition of all events
#include "WkCocos/Shop/Shop.h"

#define TAG "ShopEventHandler >>>"

void ShopEventHandler::onBillingNotSupported() {
	soomla::CCStoreUtils::logDebug(TAG, "BillingNotSupported");
	events()->emit(WkCocos::Shop::Shop::BillingNotSupported());
}

void ShopEventHandler::onBillingSupported() {
	soomla::CCStoreUtils::logDebug(TAG, "BillingSupported");
	events()->emit(WkCocos::Shop::Shop::BillingSupported());
}

void ShopEventHandler::onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
	soomla::CCStoreUtils::logDebug(TAG, "CurrencyBalanceChanged");
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ON_CURRENCY_BALANCE_CHANGED, cocos2d::CCInteger::create(balance));
	events()->emit(WkCocos::Shop::Shop::CurrencyBalanceChanged(virtualCurrency, balance, amountAdded));
}

void ShopEventHandler::onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodBalanceChanged");
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent
		(EVENT_ON_GOOD_BALANCE_CHANGED, cocos2d::CCArray::create(virtualGood, cocos2d::CCInteger::create(balance), NULL));
	//events()->emit(WkCocos::Shop::Shop::GoodBalanceChanged(virtualGood, balance, amountAdded));
}

void ShopEventHandler::onGoodEquipped(soomla::CCEquippableVG *equippableVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodEquipped");
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ON_GOOD_EQUIPPED, equippableVG);
	//events()->emit(WkCocos::Shop::Shop::GoodEquipped(equippableVG));
}

void ShopEventHandler::onGoodUnEquipped(soomla::CCEquippableVG *equippableVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodUnEquipped");
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ON_GOOD_UNEQUIPPED, equippableVG);
	//events()->emit(WkCocos::Shop::Shop::GoodUnEquipped(equippableVG));
}

void ShopEventHandler::onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodUpgrade");
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ON_GOOD_UPGRADE, virtualGood);
	//events()->emit(WkCocos::Shop::Shop::GoodUpgrade(equippableVG));
}

void ShopEventHandler::onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "ItemPurchased");
	events()->emit(WkCocos::Shop::Shop::ItemPurchased(purchasableVirtualItem));
}

void ShopEventHandler::onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "ItemPurchaseStarted");
	events()->emit(WkCocos::Shop::Shop::ItemPurchaseStarted(purchasableVirtualItem));
}

void ShopEventHandler::onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseCancelled");
	events()->emit(WkCocos::Shop::Shop::MarketPurchaseCancelled(purchasableVirtualItem));
}

void ShopEventHandler::onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchase");
	events()->emit(WkCocos::Shop::Shop::MarketPurchase(purchasableVirtualItem, token, payload));
}

void ShopEventHandler::onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseStarted");
	events()->emit(WkCocos::Shop::Shop::MarketPurchaseStarted(purchasableVirtualItem));
}

void ShopEventHandler::onMarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseVerification");
	events()->emit(WkCocos::Shop::Shop::MarketPurchaseVerification(purchasableVirtualItem));
}

void ShopEventHandler::onRestoreTransactionsStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsStarted");
	events()->emit(WkCocos::Shop::Shop::RestoreTransactionsStarted());
}

void ShopEventHandler::onRestoreTransactionsFinished(bool success) {
	soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsFinished");
	events()->emit(WkCocos::Shop::Shop::RestoreTransactionsFinished(success));
}

void ShopEventHandler::onUnexpectedErrorInStore() {
	soomla::CCStoreUtils::logDebug(TAG, "UnexpectedErrorInStore");
	events()->emit(WkCocos::Shop::Shop::UnexpectedErrorInStore());
}

void ShopEventHandler::onStoreControllerInitialized() {
	soomla::CCStoreUtils::logDebug(TAG, "StoreControllerInitialized");
	events()->emit(WkCocos::Shop::Shop::StoreControllerInitialized());
}

void ShopEventHandler::onMarketItemsRefreshed() {
	soomla::CCStoreUtils::logDebug(TAG, "MarketItemsRefreshed");
	events()->emit(WkCocos::Shop::Shop::MarketItemsRefreshed());
}

void ShopEventHandler::onMarketItemsRefreshStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "MarketItemsRefreshStarted");
	events()->emit(WkCocos::Shop::Shop::MarketItemsRefreshStarted());
}

void ShopEventHandler::onMarketItemRefreshed(soomla::CCMarketItem *mi)
{
	std::string productID = mi->getProductId()->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
                std::string marketPrice = "test";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string marketPrice = mi->getMarketPrice()->getCString();
#endif

	// TO DO: store productID and marketPrice into a std::map<string, string> to use in your app

	std::ostringstream logmsg;
	logmsg << "[SOOMLA MARKET onMarketItemRefreshed product:[" << productID << "] market price: [" << marketPrice << std::endl;
	soomla::CCStoreUtils::logDebug(TAG, logmsg.str().c_str());


	logmsg << "[SOOMLA MARKET onMarketItemRefreshed calling " << cocos2d::Director::getInstance()->getEventDispatcher() <<"->dispatchCustomEvent(EVENT_ON_MARKET_ITEM_REFRESHED)" << std::endl;
	soomla::CCStoreUtils::logDebug(TAG, logmsg.str().c_str());
    //BUG : sometimes this doesnt get called
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ON_MARKET_ITEM_REFRESHED, mi);

	events()->emit(WkCocos::Shop::Shop::MarketItemRefreshed(mi));

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ShopEventHandler::onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	std::string itemID = purchasableVirtualItem->getItemId()->getCString();

	std::ostringstream logmsg;
	logmsg << "[SOOMLA MARKET onMarketRefund] item:[" << itemID << "] " << std::endl;
	soomla::CCStoreUtils::logDebug(TAG, "MarketRefund");
	events()->emit(WkCocos::Shop::Shop::MarketRefund(purchasableVirtualItem));
}

void ShopEventHandler::onIabServiceStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "IabServiceStarted");
	events()->emit(WkCocos::Shop::Shop::IabServiceStarted());
}

void ShopEventHandler::onIabServiceStopped() {
	soomla::CCStoreUtils::logDebug(TAG, "IabServiceStopped");
	events()->emit(WkCocos::Shop::Shop::IabServiceStopped());
}
#endif
