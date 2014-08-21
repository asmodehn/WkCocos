#include "WkCocosApp/GameLogic.h"

GameLogic::GameLogic()
: m_localdatamngr(new WkCocos::LocalData::LocalDataManager())
, m_options(new MyOptions(m_localdatamngr))
//license number is in clear here as this is not a published app.
, m_player()
{
	m_shopAssets.reset(new WkCocos::Shop::ShopAssets());

	m_shopAssets->addVirtualCurrency("Static","Static Test Currency","currency_static_test");
	m_shopAssets->addVirtualCurrencyPack("Test Purchased", "Static Response Test Purchased ", GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_PURCHASED_STATIC_PRODUCT_ID, 1000);
	m_shopAssets->addVirtualCurrencyPack("Test Canceled", "Static Response Test Canceled", GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_CANCELED_STATIC_PRODUCT_ID, 1000);
	m_shopAssets->addVirtualCurrencyPack("Test Refunded", "Static Response Test Refunded", GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_REFUND_STATIC_PRODUCT_ID, 1000);
	m_shopAssets->addVirtualCurrencyPack("Test Unavailable", "Static Response Test Unavailable", GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID, 5, "currency_static_test", GOOGLE_PLAY_UNAVAILABLE_STATIC_PRODUCT_ID, 1000);

	m_shopAssets->addVirtualCurrency("Gem", "Gem Currency", "currency_gem");
	m_shopAssets->addVirtualCurrencyPack("5 Gems", "5 Shiny Gems ", "gem_5", 5, "currency_gem", "gem_5", 1000);
	m_shopAssets->addVirtualCurrencyPack("10 Gems", "10 Shiny Gems ", "gem_10", 10, "currency_gem", "gem_10", 1000);
	m_shopAssets->addVirtualCurrencyPack("25 Gems", "25 Shiny Gems ", "gem_25", 25, "currency_gem", "gem_25", 1000);
	m_shopAssets->addVirtualCurrencyPack("50 Gems", "50 Shiny Gems ", "gem_50", 50, "currency_gem", "gem_50", 1000);


	m_player.reset(new MyPlayer(m_localdatamngr, "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgckSrYT3yMLAYSS/2NVN3jtIdypsOCikdgiTr2mDu8fmwRPa3945vTPVPCOlbL3b77IYgpBf3PMOZcftGL2Jtdyk6AReLjixQzkeyaRLYK4kq9+0JYuD9V/uvqleuCw9NkzZaEOzGBU5IlFYGbXkZm6j/TPytjnJja0kTyXhiJKxzOyCsiUJ4VhLTUk4KL2py+YjPN8/MluOr+Uc/r88Rpd7M2fVH0pdqu35C2xuxLnnbCbu9xvVBPX3l/sb0srDgxdlrRY8JxkNr0mLMdmxnreDRz2aavMuXn2MS7xjB4YgbHLo75tgvTKxD1TbTtocB5VNPIg64a4hXq8rX/z2DwIDAQAB", "53CR3T", m_shopAssets));
}

GameLogic::~GameLogic()
{}

