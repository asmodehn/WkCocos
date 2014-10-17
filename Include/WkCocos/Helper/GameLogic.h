#ifndef __WKCOCOS_HELPER_GAMELOGIC_H__
#define __WKCOCOS_HELPER_GAMELOGIC_H__

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Timer/Timer.h"

namespace WkCocos
{
	namespace Helper
	{
		/**
		* Hold the GameLogic information.
		* The Game can delegate the process of updating all Managers to ONE instance of this class.
		*/
		class GameLogic
		{
		public:

			/**
			* Update loop, called by cocos.
			*/
			void Update(float deltatime);

			/**
			* Accessor to GameClock
			*/
			std::shared_ptr<Timer::Timer> getGameClock()
			{
				return  m_gameclock;
			}
			
			std::shared_ptr<WkCocos::LocalData::LocalDataManager> getLocalDataManager()
			{
				return m_localdatamngr;
			}

			std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> getOnlineDataManager()
			{
				return m_onlinedatamngr;
			}

			/**
			* Events manager to subscribe to to receive any GameLogic Events
			*/
			entityx::ptr<entityx::EventManager> gamelogic_events;
			
			/**
			* constructor for online game
			*/
			GameLogic(std::string app_access_key, std::string app_secret_key, std::function<void()> gameclock_init_cb);

			/**
			* constructor for local game
			*/
			GameLogic(std::function<void()> gameclock_init_cb);
			

		protected:
			std::shared_ptr<Timer::Timer> m_gameclock;

			std::shared_ptr<WkCocos::LocalData::LocalDataManager> m_localdatamngr;
			std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> m_onlinedatamngr;
		};
	} // namespace Helper
} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
