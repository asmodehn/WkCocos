#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Shop/Assets.h"

GameLogic::GameLogic(std::string app_access_key, std::string app_secret_key, std::function<void()> online_init_cb)
: m_player()
, m_shop()
{
	m_logic.reset(new WkCocos::Helper::GameLogic(app_access_key, app_secret_key, [=](){
		//setting up shop after logic initialized
		//careful : delayed initialization
		m_shop.reset(new WkCocos::Shop::Shop(
			"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgckSrYT3yMLAYSS/2NVN3jtIdypsOCikdgiTr2mDu8fmwRPa3945vTPVPCOlbL3b77IYgpBf3PMOZcftGL2Jtdyk6AReLjixQzkeyaRLYK4kq9+0JYuD9V/uvqleuCw9NkzZaEOzGBU5IlFYGbXkZm6j/TPytjnJja0kTyXhiJKxzOyCsiUJ4VhLTUk4KL2py+YjPN8/MluOr+Uc/r88Rpd7M2fVH0pdqu35C2xuxLnnbCbu9xvVBPX3l/sb0srDgxdlrRY8JxkNr0mLMdmxnreDRz2aavMuXn2MS7xjB4YgbHLo75tgvTKxD1TbTtocB5VNPIg64a4hXq8rX/z2DwIDAQAB"
			, "53CR3T" //license number is in clear here as this is not a published app.
			, shopInit()
		));

	}));

	//Player should be constructible independently of time based objects.
	m_player.reset(new MyPlayer(m_logic->getGameClock()
		, m_logic->getLocalDataManager()
		, [](std::string userid)  -> std::string
		{
			CCLOG("%s", userid.c_str());
			return "pass_" + userid;
		}
		, m_logic->getOnlineDataManager()
		, online_init_cb
	));

	//localData manager is created synchronously. we can alreayd access it
	m_options.reset(new MyOptions(m_logic->getLocalDataManager()));
}

GameLogic::~GameLogic()
{}

std::unique_ptr<WkCocos::Shop::Assets> GameLogic::shopInit()
{
	std::unique_ptr<WkCocos::Shop::Assets> shopAssets(new WkCocos::Shop::Assets());

	//THIS IS TOO DANGEROUS for now : if a user has a android.test purchase in his list of purchase, other purchase will fail ( with a signature fail error ).
	// => we must prevent users to use static test response.
	//If it ever happens there is code to uncomment ( just for one time use ) in iabHelper.java to clean up your list of purchase.
	/*
	shopAssets->addVirtualCurrency("Static", "Static Test Currency", "currency_static_test");
	shopAssets->addVirtualCurrencyPack("Test Purchased", "Static Response Test Purchased ", GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID, 1000);
	shopAssets->addVirtualCurrencyPack("Test Canceled", "Static Response Test Canceled", GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID, 1000);
	shopAssets->addVirtualCurrencyPack("Test Refunded", "Static Response Test Refunded", GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID, 1000);
	shopAssets->addVirtualCurrencyPack("Test Unavailable", "Static Response Test Unavailable", GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID, 1000);
	*/
	shopAssets->addVirtualCurrency("Gem", "Gem Currency", "currency_gem");
	shopAssets->addVirtualCurrencyPack("5 Gems", "5 Shiny Gems ", "gem_5", 5, "currency_gem", "gem_5", 1000);
	shopAssets->addVirtualCurrencyPack("10 Gems", "10 Shiny Gems ", "gem_10", 10, "currency_gem", "gem_10", 1000);
	shopAssets->addVirtualCurrencyPack("25 Gems", "25 Shiny Gems ", "gem_25", 25, "currency_gem", "gem_25", 1000);
	shopAssets->addVirtualCurrencyPack("50 Gems", "50 Shiny Gems ", "gem_50", 50, "currency_gem", "gem_50", 1000);

	return shopAssets;
}

//global instance
std::shared_ptr<GameLogic> g_gameLogic;

