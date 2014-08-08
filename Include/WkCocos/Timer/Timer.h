#ifndef __WKCOCOS_TIMER_TIMER_H__
#define __WKCOCOS_TIMER_TIMER_H__

#include "entityx/entityx.h"
#include "WkCocos/Timer/Comp/TimeValue.h"
//needed for classes using Timer.
#include "WkCocos/Timer/Events/TimerUpdate.h"
#include "WkCocos/Timer/Events/AlarmOff.h"

#include <ctime>

namespace WkCocos
{
	namespace Timer
	{
		class Timer
		{
		public:
			/**
			* Constructor
			*/
			Timer();

			/**
			* Destructor
			*/
			~Timer();
			
			/**
			* Setup Timer
			* @param id identifier of the timer
			* @param alarm_date date whent he alarm will be triggered
			*/
			bool setAlarm(std::string id, struct tm alarm_date);
			
			/**
			* Delete Timer
			*/
			void deleteAlarm(std::string id);
			
			void update(double dt);

			/**
			* Get Event manager
			*/
			inline entityx::ptr<entityx::EventManager> getEventManager() { return event_manager; }

			/**
			* Get Entity manager
			*/
			inline entityx::ptr<entityx::EntityManager> getEntityManager() { return entity_manager; }

			/**
			* Get System manager
			*/
			inline entityx::ptr<entityx::SystemManager> getSystemManager() { return system_manager; }

		protected:

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			std::function<void()> m_error_callback;


		};
	}// namespace Timer
}//namespace WkCocos
#endif //__WKCOCOS_TIMER_TIMER_H__