#ifndef  _APP_GAMELOGIC_H_
#define  _APP_GAMELOGIC_H_

#include "WkCocosApp/MyPlayer.h"

class GameLogic{
public:
	static GameLogic& Instance() {
		static GameLogic S;
		return S;
	}

	void setPlayer(std::string app_access_key, std::string app_secret_key)
	{
		m_player.reset(new MyPlayer(app_access_key, app_secret_key));
	}

	MyPlayer& getPlayer()
	{
		return *(m_player.get());
	}

private:
	GameLogic();
	~GameLogic();

	std::unique_ptr<MyPlayer> m_player;
};

#endif