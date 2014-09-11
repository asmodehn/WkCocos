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
			* Setup Alarm
			* @param id identifier of the alarm
			* @param alarm_date date whent he alarm will be triggered
			*/
			bool setAlarm(std::string id, struct tm alarm_date);

			/**
			* Stops the Alarm
			*/
			void stopAlarm(std::string id);
			
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

			struct advanced_time
			{
				advanced_time()
				{
					m_start_time = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					m_msecs = 0;
				};
				struct tm m_start_time;
				double m_msecs;
				void m_add(double dt)
				{
					m_msecs += dt;
					if (m_msecs > 1)
					{
						m_msecs -= 1;
						m_start_time.tm_sec += 1;
						mktime(&m_start_time);
					};
				};
			};

			tm getRunTime()
			{
				return m_app_work_time.m_start_time;
			}

			void setRunTime(std::string s_iso8601)
			{
				char* pos;
				m_app_work_time.m_start_time.tm_year = strtoul(s_iso8601.c_str(), &pos, 10) - 1900;
				m_app_work_time.m_start_time.tm_mon = strtoul(++pos, &pos, 10) - 1;
				m_app_work_time.m_start_time.tm_mday = strtoul(++pos, &pos, 10);
				m_app_work_time.m_start_time.tm_hour = strtoul(++pos, &pos, 10);
				m_app_work_time.m_start_time.tm_min = strtoul(++pos, &pos, 10);
				m_app_work_time.m_start_time.tm_sec = strtoul(++pos, &pos, 10);
				m_app_work_time.m_msecs = double(strtoul(++pos, &pos, 10)) / 1000;
			}

		protected:

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			std::function<void()> m_error_callback;

			advanced_time m_app_work_time;

		};
	}// namespace Timer
}//namespace WkCocos
#endif //__WKCOCOS_TIMER_TIMER_H__