#ifndef  _APP_GAMELOGIC_H_
#define  _APP_GAMELOGIC_H_

#include "WkCocos/Helper/GameLogic.h"
#include "WkCocos/Shop/Shop.h"

#include "WkCocosApp/MyPlayer.h"
#include "WkCocosApp/MyOptions.h"

//#include "cocos2d.h"

//TODO : This used ot be a singleton. but it is not needed, it can be just a global variable.
// => We should review design in order to include GameLogic as a Helper in Lib ( providing easy access to all managers. )
class GameLogic
{
	//delegating engine logic to Wkcocos Helper
	std::unique_ptr<WkCocos::Helper::GameLogic> m_logic;

public:
	
	void localDataError()
	{
		CCLOG("LOCAL DATA ERROR");
	}

	MyPlayer& getPlayer()
	{
		return *(m_player.get());
	}

	MyOptions& getOptions()
	{
		return *(m_options.get());
	}
	
	WkCocos::Shop::Shop& getShop()
	{
		return *(m_shop.get());
	}

	WkCocos::LocalData::LocalDataManager& getLocalDataManager()
	{
		return *(m_logic->getLocalDataManager());
	}

	WkCocos::OnlineData::OnlineDataManager& getOnlineDataManager()
	{
		return *(m_logic->getOnlineDataManager());
	}


public:
	GameLogic(std::string app_access_key, std::string app_secret_key, std::function<void()> online_init_cb);
	~GameLogic();
	
	//overall game concepts
	std::unique_ptr<MyPlayer> m_player;
	std::unique_ptr<WkCocos::Shop::Shop> m_shop;
	std::unique_ptr<MyOptions> m_options;

private:
	//static utility method to run independently of when instance is constructed.
	static std::unique_ptr<WkCocos::Shop::Assets> shopInit();
};

//global instance
extern std::shared_ptr<GameLogic> g_gameLogic;

#endif