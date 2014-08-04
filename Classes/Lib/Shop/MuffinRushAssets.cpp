//
// Created by Fedor Shubin on 5/24/13.
//


#include "WkCocos/Shop/MuffinRushAssets.h"

#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualGood.h"
#include "CCSingleUseVG.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "CCVirtualCategory.h"
#include "CCNonConsumableItem.h"
#include "CCUpgradeVG.h"
#include "CCSingleUsePackVG.h"
#include "CCEquippableVG.h"

#define MUFFIN_CURRENCY_ITEM_ID "currency_muffin"
#define TENMUFF_PACK_PRODUCT_ID "android.test.refunded"
#define FIFTYMUFF_PACK_PRODUCT_ID "android.test.canceled"
#define FOURHUNDMUFF_PACK_PRODUCT_ID "android.test.purchased"
#define THOUSANDMUFF_PACK_PRODUCT_ID "android.test.item_unavailable"
#define NO_ADDS_NONCONS_PRODUCT_ID "no_ads"

#define MUFFINCAKE_ITEM_ID "fruit_cake"
#define PAVLOVA_ITEM_ID "pavlova"
#define CHOCLATECAKE_ITEM_ID "chocolate_cake"
#define CREAMCUP_ITEM_ID "cream_cup"

namespace WkCocos
{

	namespace Shop
	{
MuffinRushAssets *MuffinRushAssets::create() {
    MuffinRushAssets *ret = new MuffinRushAssets();
    ret->autorelease();
    ret->init();

    return ret;
}

bool MuffinRushAssets::init() {
    /** Virtual Currencies **/
	soomla::CCVirtualCurrency *muffinCurrency = soomla::CCVirtualCurrency::create(
            cocos2d::CCString::create("Muffins"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID)
    );


    /** Virtual Currency Packs **/

    soomla::CCVirtualCurrencyPack *tenmuffPack = soomla::CCVirtualCurrencyPack::create(
		cocos2d::CCString::create("10 Muffins"),                                   // name
		cocos2d::CCString::create("Test refund of an item"),                       // description
		cocos2d::CCString::create("muffins_10"),                                   // item id
		cocos2d::CCInteger::create(10),												// number of currencies in the pack
		cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
		soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(TENMUFF_PACK_PRODUCT_ID), cocos2d::CCDouble::create(0.99))
    );

    soomla::CCVirtualCurrencyPack *fiftymuffPack = soomla::CCVirtualCurrencyPack::create(
            cocos2d::CCString::create("50 Muffins"),                                   // name
            cocos2d::CCString::create("Test cancellation of an item"),                 // description
            cocos2d::CCString::create("muffins_50"),                                   // item id
            cocos2d::CCInteger::create(50),												// number of currencies in the pack
            cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(FIFTYMUFF_PACK_PRODUCT_ID), cocos2d::CCDouble::create(1.99))
    );

    soomla::CCVirtualCurrencyPack *fourhundmuffPack = soomla::CCVirtualCurrencyPack::create(
            cocos2d::CCString::create("400 Muffins"),                                   // name
            cocos2d::CCString::create("Test purchase of an item"),                 // description
            cocos2d::CCString::create("muffins_400"),                                   // item id
            cocos2d::CCInteger::create(400),												// number of currencies in the pack
            cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(FOURHUNDMUFF_PACK_PRODUCT_ID), cocos2d::CCDouble::create(4.99))
    );

    soomla::CCVirtualCurrencyPack *thousandmuffPack = soomla::CCVirtualCurrencyPack::create(
            cocos2d::CCString::create("1000 Muffins"),                                   // name
            cocos2d::CCString::create("Test item unavailable"),                 // description
            cocos2d::CCString::create("muffins_1000"),                                   // item id
            cocos2d::CCInteger::create(1000),												// number of currencies in the pack
            cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID),                        // the currency associated with this pack
            soomla::CCPurchaseWithMarket::create(cocos2d::CCString::create(THOUSANDMUFF_PACK_PRODUCT_ID), cocos2d::CCDouble::create(8.99))
    );

    /** Virtual Goods **/

    soomla::CCVirtualGood *muffincakeGood = soomla::CCSingleUseVG::create(
            cocos2d::CCString::create("Fruit Cake"),                                       // name
            cocos2d::CCString::create("Customers buy a double portion on each purchase of this cake"), // description
            cocos2d::CCString::create("fruit_cake"),                                       // item id
            soomla::CCPurchaseWithVirtualItem::create(
                    cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(225)
            ) // the way this virtual good is purchased
    );

    soomla::CCVirtualGood *pavlovaGood = soomla::CCSingleUseVG::create(
            cocos2d::CCString::create("Pavlova"),                                       // name
            cocos2d::CCString::create("Gives customers a sugar rush and they call their friends"), // description
            cocos2d::CCString::create("pavlova"),                                       // item id
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(175)) // the way this virtual good is purchased
    );

    soomla::CCVirtualGood *tenPavlovaGoods = soomla::CCSingleUsePackVG::create(
            cocos2d::CCString::create("pavlova"), cocos2d::CCInteger::create(10),
            cocos2d::CCString::create("10 Pavlova"),
            cocos2d::CCString::create("Gives customers a sugar rush and they call their friends"),
            cocos2d::CCString::create("pavlova_10"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(1750)));

    soomla::CCVirtualGood *choclatecakeGood = soomla::CCSingleUseVG::create(
            cocos2d::CCString::create("Chocolate Cake"),                                       // name
            cocos2d::CCString::create("A classic cake to maximize customer satisfaction"), // description
            cocos2d::CCString::create("chocolate_cake"),                                       // item id
            soomla::CCPurchaseWithVirtualItem::create(
                    cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(250)
            ) // the way this virtual good is purchased
    );

    soomla::CCVirtualGood *creamcupGood = soomla::CCSingleUseVG::create(
            cocos2d::CCString::create("Cream Cup"),                                       // name
            cocos2d::CCString::create("Increase bakery reputation with this original pastry"), // description
            cocos2d::CCString::create("cream_cup"),                                       // item id
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(50)
            ) // the way this virtual good is purchased
    );

    soomla::CCVirtualGood *tenCreamcupGoods = soomla::CCSingleUsePackVG::create(
            cocos2d::CCString::create("cream_cup"), cocos2d::CCInteger::create(10),
            cocos2d::CCString::create("10 Cream Cup"),
            cocos2d::CCString::create("Increase bakery reputation with this original pastry"),
            cocos2d::CCString::create("cream_cup_10"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(500)));

    soomla::CCVirtualGood *showRoomGood =
            soomla::CCLifetimeVG::create(cocos2d::CCString::create("Show Room"), // name
                    cocos2d::CCString::create("Show Room "), // description
                    cocos2d::CCString::create("show_room"), // item id
                    soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(100)));

    soomla::CCVirtualGood *showRoomGood0 = soomla::CCUpgradeVG::create(
            cocos2d::CCString::create("show_room"),
            NULL,
            cocos2d::CCString::create("show_room_1"),
            cocos2d::CCString::create("Show Room L0"),
            cocos2d::CCString::create(""),
            cocos2d::CCString::create("show_room_0"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(220)));
	
    soomla::CCVirtualGood *showRoomGood1 = soomla::CCUpgradeVG::create(
            cocos2d::CCString::create("show_room"),
            cocos2d::CCString::create("show_room_0"),
            cocos2d::CCString::create("show_room_2"),
            cocos2d::CCString::create("Show Room L1"),
            cocos2d::CCString::create(""),
            cocos2d::CCString::create("show_room_1"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(220)));

	soomla::CCVirtualGood *showRoomGood2 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("show_room"),
			cocos2d::CCString::create("show_room_1"),
			cocos2d::CCString::create("show_room_3"),
			cocos2d::CCString::create("Show Room L2"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("show_room_2"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(220)));

	soomla::CCVirtualGood *showRoomGood3 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("show_room"),
			cocos2d::CCString::create("show_room_2"),
			cocos2d::CCString::create("show_room_4"),
			cocos2d::CCString::create("Show Room L3"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("show_room_3"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(220)));

	soomla::CCVirtualGood *showRoomGood4 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("show_room"),
			cocos2d::CCString::create("show_room_3"),
			NULL,
			cocos2d::CCString::create("Show Room L4"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("show_room_4"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(220)));
	
    soomla::CCVirtualGood *deliveryVehicleGood =
            soomla::CCLifetimeVG::create(cocos2d::CCString::create("Delivery Vehicle"), // name
                    cocos2d::CCString::create("Delivery Vehicle"), // description
                    cocos2d::CCString::create("delivery_vehicle"), // item id
                    soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));

    soomla::CCVirtualGood *deliveryVehicleGood0 = soomla::CCUpgradeVG::create(
            cocos2d::CCString::create("delivery_vehicle"),
            NULL,
            cocos2d::CCString::create("delivery_vehicle_1"),
            cocos2d::CCString::create("Delivery Vehicle 0"),
            cocos2d::CCString::create(""),
            cocos2d::CCString::create("delivery_vehicle_0"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));

    soomla::CCVirtualGood *deliveryVehicleGood1 = soomla::CCUpgradeVG::create(
            cocos2d::CCString::create("delivery_vehicle"),
            cocos2d::CCString::create("delivery_vehicle_0"),
            cocos2d::CCString::create("delivery_vehicle_2"),
            cocos2d::CCString::create("Delivery Vehicle 1"),
            cocos2d::CCString::create(""),
            cocos2d::CCString::create("delivery_vehicle_1"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));

    soomla::CCVirtualGood *deliveryVehicleGood2 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("delivery_vehicle"),
			cocos2d::CCString::create("delivery_vehicle_1"),
			cocos2d::CCString::create("delivery_vehicle_3"),
			cocos2d::CCString::create("Delivery Vehicle 2"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("delivery_vehicle_2"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));

    soomla::CCVirtualGood *deliveryVehicleGood3 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("delivery_vehicle"),
			cocos2d::CCString::create("delivery_vehicle_2"),
			cocos2d::CCString::create("delivery_vehicle_4"),
			cocos2d::CCString::create("Delivery Vehicle 3"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("delivery_vehicle_3"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));

	soomla::CCVirtualGood *deliveryVehicleGood4 = soomla::CCUpgradeVG::create(
			cocos2d::CCString::create("delivery_vehicle"),
			cocos2d::CCString::create("delivery_vehicle_3"),
			NULL,
			cocos2d::CCString::create("Delivery Vehicle 4"),
			cocos2d::CCString::create(""),
			cocos2d::CCString::create("delivery_vehicle_4"),
			soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(20)));
	
    soomla::CCVirtualGood *fatCatGood = soomla::CCEquippableVG::create(
            cocos2d::CCInteger::create(soomla::CCEquippableVG::kCategory),
            cocos2d::CCString::create("Fat Cat"),
            cocos2d::CCString::create("Fat cat description"),
            cocos2d::CCString::create("fat_cat"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(27)));

    soomla::CCVirtualGood *happiHippoGood = soomla::CCEquippableVG::create(
            cocos2d::CCInteger::create(soomla::CCEquippableVG::kCategory),
            cocos2d::CCString::create("Happi Hippo"),
            cocos2d::CCString::create("Happi Hippo description"),
            cocos2d::CCString::create("happi_hippo"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(44)));

    soomla::CCVirtualGood *funkeyMonkeyGood = soomla::CCEquippableVG::create(
            cocos2d::CCInteger::create(soomla::CCEquippableVG::kCategory),
            cocos2d::CCString::create("Funkey Monkey"),
            cocos2d::CCString::create("Funkey Monkey description"),
            cocos2d::CCString::create("funkey_monkey"),
            soomla::CCPurchaseWithVirtualItem::create(cocos2d::CCString::create(MUFFIN_CURRENCY_ITEM_ID), cocos2d::CCInteger::create(35)));


    /** Virtual Categories **/

    soomla::CCVirtualCategory *cakes = soomla::CCVirtualCategory::create(
            cocos2d::CCString::create("Cakes"),
            cocos2d::CCArray::create(
                    cocos2d::CCString::create(MUFFINCAKE_ITEM_ID),
                    cocos2d::CCString::create(PAVLOVA_ITEM_ID),
                    cocos2d::CCString::create("pavlova_10"),
                    cocos2d::CCString::create(CHOCLATECAKE_ITEM_ID),
                    cocos2d::CCString::create(CREAMCUP_ITEM_ID),
                    cocos2d::CCString::create("cream_cup_10"),
                    NULL));

    soomla::CCVirtualCategory *upgrades = soomla::CCVirtualCategory::create(
            cocos2d::CCString::create("Upgrades"),
            cocos2d::CCArray::create(cocos2d::CCString::create("show_room_0"),
							cocos2d::CCString::create("show_room_1"),
							cocos2d::CCString::create("show_room_2"),
							cocos2d::CCString::create("show_room_3"),
							cocos2d::CCString::create("show_room_4"),
							cocos2d::CCString::create("delivery_vehicle_0"),
							cocos2d::CCString::create("delivery_vehicle_1"),
							cocos2d::CCString::create("delivery_vehicle_2"),
							cocos2d::CCString::create("delivery_vehicle_3"),
							cocos2d::CCString::create("delivery_vehicle_4"),
							NULL));

    soomla::CCVirtualCategory *characters = soomla::CCVirtualCategory::create(
            cocos2d::CCString::create("Characters"),
            cocos2d::CCArray::create(cocos2d::CCString::create("fat_cat"), cocos2d::CCString::create("happi_hippo"), cocos2d::CCString::create("funkey_monkey"), NULL));


    /** Google MANAGED Items **/

    soomla::CCNonConsumableItem *noAdsNoncons = soomla::CCNonConsumableItem::create(
            cocos2d::CCString::create("No Ads"),
            cocos2d::CCString::create("Test purchase of MANAGED item."),
            cocos2d::CCString::create("no_ads"),
            soomla::CCPurchaseWithMarket::createWithMarketItem(soomla::CCMarketItem::create(
                    cocos2d::CCString::create(NO_ADDS_NONCONS_PRODUCT_ID),
                    cocos2d::CCInteger::create(soomla::CCMarketItem::NONCONSUMABLE), cocos2d::CCDouble::create(1.99))
            )
    );

    mCurrencies = cocos2d::CCArray::create(muffinCurrency, NULL);
    mCurrencies->retain();

    mGoods = cocos2d::CCArray::create(muffincakeGood, pavlovaGood, tenPavlovaGoods, choclatecakeGood, creamcupGood, tenCreamcupGoods,
							 showRoomGood, showRoomGood0, showRoomGood1, showRoomGood2, showRoomGood3, showRoomGood4,
							 deliveryVehicleGood, deliveryVehicleGood0, deliveryVehicleGood1, deliveryVehicleGood2,
							 deliveryVehicleGood3, deliveryVehicleGood4, fatCatGood, happiHippoGood, funkeyMonkeyGood, NULL);
    mGoods->retain();

    mCurrencyPacks = cocos2d::CCArray::create(tenmuffPack, fiftymuffPack, fourhundmuffPack, thousandmuffPack, NULL);
    mCurrencyPacks->retain();

    mCategories = cocos2d::CCArray::create(cakes, upgrades, characters, NULL);
    mCategories->retain();

    mNonConsumableItems = cocos2d::CCArray::create(noAdsNoncons, NULL);
    mNonConsumableItems->retain();

    return true;
}

MuffinRushAssets::~MuffinRushAssets() {
    CC_SAFE_RELEASE(mCurrencies);
    CC_SAFE_RELEASE(mGoods);
    CC_SAFE_RELEASE(mCurrencyPacks);
    CC_SAFE_RELEASE(mCategories);
    CC_SAFE_RELEASE(mNonConsumableItems);
}

int MuffinRushAssets::getVersion() {
    return 0;
}

cocos2d::CCArray *MuffinRushAssets::getCurrencies() {
    return mCurrencies;
}

cocos2d::CCArray *MuffinRushAssets::getGoods() {
    return mGoods;
}

cocos2d::CCArray *MuffinRushAssets::getCurrencyPacks() {
    return mCurrencyPacks;
}

cocos2d::CCArray *MuffinRushAssets::getCategories() {
    return mCategories;
}

cocos2d::CCArray *MuffinRushAssets::getNonConsumableItems() {
    return mNonConsumableItems;
}


	} //namespace Shop
} // namespace WkCocos
