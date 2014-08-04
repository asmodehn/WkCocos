#ifndef __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
#define __WKCOCOS_SHOP_MUFFINRUSHASSETS_H__

#include "entityx/entityx.h"

#include "CCStoreAssets.h"
#include "cocos2d.h"

namespace WkCocos
{
	namespace Shop
	{

		class MuffinRushAssets : public soomla::CCStoreAssets {
			cocos2d::Array *mCurrencies;
			cocos2d::Array *mGoods;
			cocos2d::Array *mCurrencyPacks;
			cocos2d::Array *mCategories;
			cocos2d::Array *mNonConsumableItems;
		public:
			MuffinRushAssets() :
				mCurrencies(NULL),
				mGoods(NULL),
				mCurrencyPacks(NULL),
				mCategories(NULL),
				mNonConsumableItems(NULL)
			{}

			virtual ~MuffinRushAssets();

			static MuffinRushAssets *create();
			bool init();

			virtual int getVersion();

			virtual cocos2d::CCArray *getCurrencies();

			virtual cocos2d::CCArray *getGoods();

			virtual cocos2d::CCArray *getCurrencyPacks();

			virtual cocos2d::CCArray *getCategories();

			virtual cocos2d::CCArray *getNonConsumableItems();
		};

	} //namespace Shop
}//namespace WkCocos

#endif //__WKCOCOS_SHOP_MUFFINRUSHASSETS_H__
