#ifndef  _APP_GAMELOGIC_H_
#define  _APP_GAMELOGIC_H_

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Shop/Shop.h"

#include "WkCocosApp/MyPlayer.h"
#include "WkCocosApp/MyOptions.h"

//#include "cocos2d.h"

//TODO : This used ot be a singleton. but it is not needed, it can be just a global variable.
// => We should review design in order to include GameLogic as a Helper in Lib ( providing easy access to all managers. )
class GameLogic{
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
		return *(m_localdatamngr.get());
	}

	WkCocos::OnlineData::OnlineDataManager& getOnlineDataManager()
	{
		return *(m_onlinedatamngr.get());
	}

public:
	GameLogic(std::string app_access_key, std::string app_secret_key, std::function<void()> data_load_cb);
	~GameLogic();
	
	//overall game features ( shared between concepts )
	std::shared_ptr<WkCocos::LocalData::LocalDataManager> m_localdatamngr;
	std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> m_onlinedatamngr;
	

	//overall game concepts
	std::unique_ptr<MyPlayer> m_player;
	std::unique_ptr<WkCocos::Shop::Shop> m_shop;
	std::unique_ptr<MyOptions> m_options;

};

//global instance
extern std::shared_ptr<GameLogic> g_gameLogic;

#endif