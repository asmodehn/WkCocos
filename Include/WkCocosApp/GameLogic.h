#ifndef  _APP_GAMELOGIC_H_
#define  _APP_GAMELOGIC_H_

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Shop/Shop.h"

#include "WkCocosApp/MyPlayer.h"
#include "WkCocosApp/MyOptions.h"

//#include "cocos2d.h"

class GameLogic{
public:
	static GameLogic& Instance() {
		static GameLogic S;
		return S;
	}
	
	void connectApp42(std::string app_access_key, std::string app_secret_key, std::function<void()> online_init_cb)
	{
		m_onlinedatamngr.reset(new WkCocos::OnlineData::OnlineDataManager(app_access_key, app_secret_key));
		//TMP forcing local save
		m_player->setOnlineDataManager(m_onlinedatamngr, online_init_cb);
	}

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


private:
	GameLogic();
	~GameLogic();
	
	//overall game features ( shared between concepts )
	std::shared_ptr<WkCocos::LocalData::LocalDataManager> m_localdatamngr;
	std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> m_onlinedatamngr;
	

	//overall game concepts
	std::unique_ptr<MyPlayer> m_player;
	std::unique_ptr<WkCocos::Shop::Shop> m_shop;
	std::unique_ptr<MyOptions> m_options;

};

#endif