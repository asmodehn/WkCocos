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

		/**
		* error callbacks for data managers
		*/
		void local_save_error_callback();
		void online_save_error_callback();

	protected:
		Player(std::string app_access_key,std::string app_secret_key);

		static bool m_bSingletonCreating;

		std::string m_user;
		std::string m_passwd;

		LocalData::LocalDataManager m_localdata;
		OnlineData::OnlineDataManager m_onlinedata;

	};

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
