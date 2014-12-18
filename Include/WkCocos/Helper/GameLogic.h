#ifndef __WKCOCOS_HELPER_GAMELOGIC_H__
#define __WKCOCOS_HELPER_GAMELOGIC_H__

#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Timer/Timer.h"
#include "WkCocos/Actor.h"

namespace WkCocos
{
	namespace Helper
	{
		/**
		* Hold the GameLogic information.
		* The Game can delegate the process of updating all Managers to ONE instance of this class.
		*/
		class GameLogic : public WkCocos::Actor
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

			struct TimerInit : public WkCocos::Event<TimerInit>
			{
			    TimerInit(WkCocos::ActorID id)
			    : WkCocos::Event<TimerInit>(id)
                {
			    }
			};

			/**
			* constructor for online game
			* This will trigger TimerInit upon completion
			*/
			GameLogic(std::string app_access_key, std::string app_secret_key);

			/**
			* constructor for local game
			* This will trigger TimerInit upon completion
			*/
			GameLogic();


		protected:
			std::shared_ptr<Timer::Timer> m_gameclock;

			std::shared_ptr<WkCocos::LocalData::LocalDataManager> m_localdatamngr;
			std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> m_onlinedatamngr;
		};
	} // namespace Helper
} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
