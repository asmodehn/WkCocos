#include "WkCocos/Timer/Timer.h"

#include "WkCocos/Timer/Comp/TimeValue.h"

#include "WkCocos/Timer/Systems/Alarm.h"

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

			system_manager->add<Systems::Alarm>();
			system_manager->configure();
		}

		Timer::~Timer()
		{
		}

		/**
		* Setup Alarm
		*/
		bool Timer::setAlarm(std::string id, struct tm alarm_date, std::function<void(std::string id, std::string msecs_left)> update_cb)
		{
			//check unicity of id
			entityx::ptr<Comp::ID> eid;
			for (auto entity : entity_manager->entities_with_components(eid))
			{
				if (id == eid->m_id)
				{
					entity.remove<Comp::Callback>();
					entity.assign<Comp::Callback>(update_cb);
					entity.remove<Comp::Alarm>();
					entity.assign<Comp::Alarm>(alarm_date);
					return true;
				}
			}

			//creating new entity with unique id
			auto entity = entity_manager->create();
			entity.assign<Comp::ID>(id);
			entity.assign<Comp::Callback>(update_cb);
			entity.assign<Comp::Alarm>(alarm_date);
			return true;
		}
		
		/**
		* Delete Timer
		*/
		void Timer::deleteAlarm(std::string id)
		{
			entityx::ptr<Comp::ID> eid;
			for (auto entity : entity_manager->entities_with_components(eid))
			{
				if (id == eid->m_id)
				{
					entity.destroy();
				}
			}
		}

		void Timer::update(double dt) 
		{
			system_manager->update<Systems::Alarm>(dt);
		}

	} //namespace Timer
}//namespace WkCocos

