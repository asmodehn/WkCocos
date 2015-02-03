#include "WkCocos/Helper/GameLogic.h"

namespace WkCocos
{
	namespace Helper
	{
		GameLogic::GameLogic()
		{
			//registering gamelogic class in cocos update loop
			cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&GameLogic::Update, this, std::placeholders::_1), this, 1.f / 15, false, "gamelogic_update");

			m_localdatamngr.reset(new WkCocos::LocalData::LocalDataManager());

			m_gameclock.reset(new WkCocos::Timer::Timer());

			m_gameclock->getEventManager()->emit<Timer::Events::TimerInit>();

			m_saveManager.reset(new SaveManager());
			m_saveManager->configure();
		}

		GameLogic::GameLogic(std::string app_access_key, std::string app_secret_key)
		{
			//registering gamelogic class in cocos update loop
			cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&GameLogic::Update, this, std::placeholders::_1), this, 1.f / 15, false, "gamelogic_update");

			m_localdatamngr.reset(new WkCocos::LocalData::LocalDataManager());

			m_onlinedatamngr.reset(new WkCocos::OnlineData::OnlineDataManager(app_access_key, app_secret_key));

			m_gameclock.reset(new WkCocos::Timer::Timer());

			m_onlinedatamngr->getServerTime([=](std::string s_iso8601, bool sertimeTimeValid){
				m_gameclock->setTime(s_iso8601, sertimeTimeValid);

				m_gameclock->getEventManager()->emit<Timer::Events::TimerInit>();
			});

			m_saveManager.reset(new SaveManager());
			m_saveManager->configure();
		}


		void GameLogic::Update(float deltatime)
		{
			if (m_gameclock)
			{
				m_gameclock->update(deltatime);
			}
			if (m_localdatamngr)
			{
				m_localdatamngr->update(deltatime);
			}
			if (m_onlinedatamngr)
			{
				m_onlinedatamngr->update(deltatime);
			}
		}

	} // namespace Helper
} // namespace WkCocos
