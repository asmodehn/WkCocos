#include "WkCocos/Helper/GameLogic.h"

namespace WkCocos
{
	namespace Helper
	{
		GameLogic::GameLogic(std::function<void()> gameclock_init_cb)
		{
			//registering gamelogic class in cocos update loop
			cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&GameLogic::Update, this, std::placeholders::_1), this, 1.f / 15, false, "gamelogic_update");

			m_localdatamngr.reset(new WkCocos::LocalData::LocalDataManager());

			m_gameclock.reset(new WkCocos::Timer::Timer());

			if (gameclock_init_cb) gameclock_init_cb();
		}

		GameLogic::GameLogic(std::string app_access_key, std::string app_secret_key, std::function<void()> gameclock_init_cb)
		{
			//registering gamelogic class in cocos update loop
			cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&GameLogic::Update, this, std::placeholders::_1), this, 1.f / 15, false, "gamelogic_update");
			
			m_localdatamngr.reset(new WkCocos::LocalData::LocalDataManager());

			m_onlinedatamngr.reset(new WkCocos::OnlineData::OnlineDataManager(app_access_key, app_secret_key));

			m_gameclock.reset(new WkCocos::Timer::Timer());

			m_onlinedatamngr->getServerTime([=](std::string s_iso8601){
				m_gameclock->setTime(s_iso8601);

				if (gameclock_init_cb) gameclock_init_cb();
			});
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
