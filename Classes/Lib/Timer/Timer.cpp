#include "Wkcocos/Timer/Timer.h"

#include "Wkcocos/Timer/Comp/TimeValue.h"

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

			//system_manager->add<Systems::JSONReader>();

			system_manager->configure();
		}

		Timer::~Timer()
		{
		}

		/**
		* Setup Timer
		*/
		bool Timer::setTimer(std::string id, unsigned long msecs, std::function<void(std::string id, unsigned long msecs_elapsed)> update_cb)
		{
			//check unicity of id
			entityx::ptr<Comp::ID> eid;
			for (auto entity : entity_manager->entities_with_components(eid))
			{
				if (id == eid->m_id)
				{
					entity.remove<Comp::Progress>();
					entity.assign<Comp::Progress>(msecs);
					entity.remove<Comp::Callback>();
					entity.assign<Comp::Callback>(update_cb);
					return true;
				}
			}

			//creating new entity with unique id
			auto entity = entity_manager->create();
			entity.assign<Comp::ID>(id);
			entity.assign<Comp::Progress>(msecs);
			entity.assign<Comp::Callback>(update_cb);
			return true;
		}

		/**
		* StartTimer
		*/
		bool Timer::startTimer(std::string id)
		{
			entityx::ptr<Comp::ID> eid;
			entityx::ptr<Comp::Progress> eprog;
			for (auto entity : entity_manager->entities_with_components(eid, eprog))
			{
				if (id == eid->m_id)
				{
					eprog->m_started = true;
					return true;
				}
			}
			return false;
		}

		/**
		* Stop Timer
		*/
		void Timer::stopTimer(std::string id)
		{
			entityx::ptr<Comp::ID> eid;
			entityx::ptr<Comp::Progress> eprog;
			for (auto entity : entity_manager->entities_with_components(eid, eprog))
			{
				if (id == eid->m_id)
				{
					eprog->m_started = false;
				}
			}
		}

		/**
		* Delete Timer
		*/
		void Timer::deleteTimer(std::string id)
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


		///Get Current Time
		struct tm Timer::getLocalTime()
		{
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime);
			CCLOG("Current local time and date: %s", asctime(timeinfo));

			//Copy of POD tm
			return *timeinfo;
		}

		///Get Current Time
		struct tm Timer::getUTCTime()
		{
			time_t rawtime;
			struct tm * ptm;

			time(&rawtime);

			ptm = gmtime(&rawtime);

			CCLOG("Current UTC time and date: %s", asctime(ptm));

			//Copy of POD tm
			return *ptm;
		}

		void Timer::update(double dt) 
		{
			//system_manager->update<Systems::JSONWriter>(dt);
		}

	} //namespace Timer
}//namespace WkCocos

