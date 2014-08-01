#ifndef  _APP_GAMELOGIC_H_
#define  _APP_GAMELOGIC_H_

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

#include "WkCocosApp/MyPlayer.h"
#include "WkCocosApp/MyOptions.h"

//#include "cocos2d.h"

class GameLogic{
public:
	static GameLogic& Instance() {
		static GameLogic S;
		return S;
	}
	
	bool connectApp42(std::string app_access_key, std::string app_secret_key)
	{
		m_onlinedatamngr.reset(new WkCocos::OnlineData::OnlineDataManager(app_access_key, app_secret_key));
		m_player->setOnlineDataManager(m_onlinedatamngr);

		//assumes creation succeeded
		return true;
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

private:
	GameLogic();
	~GameLogic();
	
	//overall game features ( shared between concepts )
	std::shared_ptr<WkCocos::LocalData::LocalDataManager> m_localdatamngr;
	std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> m_onlinedatamngr;
	
	//overall game concepts
	std::unique_ptr<MyPlayer> m_player;
	std::unique_ptr<MyOptions> m_options;
};

#endif