#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/Utils/UUID.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

#include <string>

namespace WkCocos
{
	/**
	* Hold the player information.
	*/
	class Player
	{
	public:

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);
		
		void setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata);

	protected:
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata);

		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;

	};

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
