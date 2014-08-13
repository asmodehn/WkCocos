#include "WkCocos/Shop/ShopEventHandler.h"
#include "cocos2d.h"

#define TAG "ShopEventHandler >>>"

void ShopEventHandler::onBillingNotSupported() {
	soomla::CCStoreUtils::logDebug(TAG, "BillingNotSupported");
}

void ShopEventHandler::onBillingSupported() {
	soomla::CCStoreUtils::logDebug(TAG, "BillingSupported");
}

void ShopEventHandler::onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
	soomla::CCStoreUtils::logDebug(TAG, "CurrencyBalanceChanged");
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification
		(EVENT_ON_CURRENCY_BALANCE_CHANGED, cocos2d::CCInteger::create(balance));
}

void ShopEventHandler::onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodBalanceChanged");
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification
		(EVENT_ON_GOOD_BALANCE_CHANGED, cocos2d::CCArray::create(virtualGood, cocos2d::CCInteger::create(balance), NULL));
}

void ShopEventHandler::onGoodEquipped(soomla::CCEquippableVG *equippableVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodEquipped");
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_EQUIPPED, equippableVG);
}

void ShopEventHandler::onGoodUnEquipped(soomla::CCEquippableVG *equippableVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodUnEquipped");
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UNEQUIPPED, equippableVG);
}

void ShopEventHandler::onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG) {
	soomla::CCStoreUtils::logDebug(TAG, "GoodUpgrade");
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UPGRADE, virtualGood);
}

void ShopEventHandler::onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "ItemPurchased");
}

void ShopEventHandler::onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "ItemPurchaseStarted");
}

void ShopEventHandler::onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseCancelled");
}

void ShopEventHandler::onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchase");
}

void ShopEventHandler::onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseStarted");
}

void ShopEventHandler::onMarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseVerification");
}

void ShopEventHandler::onRestoreTransactionsStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsStarted");
}

void ShopEventHandler::onRestoreTransactionsFinished(bool success) {
	soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsFinished");
}

void ShopEventHandler::onUnexpectedErrorInStore() {
	soomla::CCStoreUtils::logDebug(TAG, "UnexpectedErrorInStore");
}

void ShopEventHandler::onStoreControllerInitialized() {
	soomla::CCStoreUtils::logDebug(TAG, "StoreControllerInitialized");
}

void ShopEventHandler::onMarketItemsRefreshed() {
	soomla::CCStoreUtils::logDebug(TAG, "MarketItemsRefreshed");
}

void ShopEventHandler::onMarketItemsRefreshStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "MarketItemsRefreshStarted");
}

void ShopEventHandler::onMarketItemRefreshed(soomla::CCMarketItem *mi)
{
	std::string productID = mi->getProductId()->getCString();
	std::string marketPrice = mi->getMarketPrice()->getCString();

	// TO DO: store productID and marketPrice into a std::map<string, string> to use in your app

	std::ostringstream logmsg;
	logmsg << "[SOOMLA MARKET onMarketItemRefreshed] product:[" << productID << "] market price: [" << marketPrice << std::endl;

	soomla::CCStoreUtils::logDebug(TAG, logmsg.str().c_str());
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ShopEventHandler::onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
	soomla::CCStoreUtils::logDebug(TAG, "MarketRefund");
}

void ShopEventHandler::onIabServiceStarted() {
	soomla::CCStoreUtils::logDebug(TAG, "IabServiceStarted");
}

void ShopEventHandler::onIabServiceStopped() {
	soomla::CCStoreUtils::logDebug(TAG, "IabServiceStopped");
}
#endif
