#include "WkCocos/Timer/Timer.h"

#include "WkCocos/Timer/Comp/TimeValue.h"

#include "WkCocos/Timer/Systems/AlarmDestructor.h"
#include "WkCocos/Timer/Systems/AlarmRinger.h"
#include "WkCocos/Timer/Systems/TimeUpdater.h"

#include "cocos/cocos2d.h"

namespace WkCocos
{
	namespace Timer
	{
		Timer::Timer()
			: event_manager(entityx::EventManager::make())
			, entity_manager(entityx::EntityManager::make(event_manager))
			, system_manager(entityx::SystemManager::make(entity_manager, event_manager))
		{

			system_manager->add<Systems::TimeUpdater>();
			system_manager->add<Systems::AlarmRinger>();
			system_manager->add<Systems::AlarmDestructor>();
			system_manager->configure();

			event_manager->subscribe<WkCocos::Timer::Events::TimerInit>(*this);
		}

		Timer::~Timer()
		{
		}

		/**
		* Setup Alarm
		*/
		bool Timer::setAlarm(std::string id, struct tm alarm_date)
		{
			//check unicity of id
			entityx::ptr<Comp::ID> eid;
			for (auto entity : entity_manager->entities_with_components(eid))
			{
				if (id == eid->m_id)
				{
					entity.remove<Comp::Alarm>();
					entity.assign<Comp::Alarm>(alarm_date, m_time.m_server_time, m_time.m_msecs);
					// Timer was stopped but recreate in the same frame, we don't want to delete it anymore.
					if (entity.component<Comp::Stopped>())
					{
						entity.remove<Comp::Stopped>();
					}
					return true;
				}
			}

			//creating new entity with unique id
			auto entity = entity_manager->create();
			entity.assign<Comp::ID>(id);
			entity.assign<Comp::Alarm>(alarm_date, m_time.m_server_time, m_time.m_msecs);
			return true;
		}

		double Timer::getAlarmTimeLeft(std::string id)
		{
			entityx::ptr<Comp::ID> eid;
			entityx::ptr<Comp::Alarm> alarm;
			for (auto entity : entity_manager->entities_with_components(eid, alarm))
			{
				if (id == eid->m_id)
				{
					time_t now = mktime(&alarm->m_now);
					time_t end = mktime(&alarm->m_end);
					return difftime(end, now);
				}
			}
			return 0.f;
		}
		
		/**
		* stops Timer
		*/
		void Timer::stopAlarm(std::string id)
		{
			entityx::ptr<Comp::ID> eid;
			for (auto entity : entity_manager->entities_with_components(eid))
			{
				if (id == eid->m_id)
				{
					entity.assign<Comp::Stopped>();
				}
			}
		}

		void Timer::update(double dt) 
		{
			m_time.m_add(dt);
			system_manager->update<Systems::TimeUpdater>(dt);
			system_manager->update<Systems::AlarmRinger>(dt);
			system_manager->update<Systems::AlarmDestructor>(dt);
		}

		///Get Current Time
		struct tm Timer::getDeviceLocalTime()
		{
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime);
			//CCLOG("Current local time and date: %s", asctime(timeinfo));

			//Copy of POD tm
			return *timeinfo;
		}

		///Get Current Time
		struct tm Timer::getDeviceUTCTime()
		{
			time_t rawtime;
			struct tm * ptm;

			time(&rawtime);

			ptm = gmtime(&rawtime);

			//CCLOG("Current UTC time and date: %s", asctime(ptm));

			//Copy of POD tm
			return *ptm;
		}

		void Timer::receive(WkCocos::Timer::Events::TimerInit const & ti)
		{
			entityx::ptr<Comp::ID> id;
			entityx::ptr<Comp::Alarm> alarm;
			for (auto entity : entity_manager->entities_with_components(id, alarm))
			{
				alarm->m_now= m_time.m_server_time;
			}
		
		}

	} //namespace Timer
}//namespace WkCocos

