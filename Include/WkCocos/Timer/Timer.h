#ifndef __WKCOCOS_TIMER_TIMER_H__
#define __WKCOCOS_TIMER_TIMER_H__

#include "entityx/entityx.h"
#include "WkCocos/Timer/Comp/TimeValue.h"
//needed for classes using Timer.
#include "WkCocos/Timer/Events/TimerUpdate.h"
#include "WkCocos/Timer/Events/AlarmOff.h"
#include "WkCocos/Timer/Events/TimerInit.h"

#include <ctime>

namespace WkCocos
{
	namespace Timer
	{
		class Timer : public entityx::Receiver<Timer>
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
			* Get Alarm time left.
			* @param id identifier of the alarm
			* @return struct of time left
			*/
			double getAlarmTimeLeft(std::string id);

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
					m_server_time = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					m_local_time = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					m_msecs = 0;
				};
				struct tm m_server_time;
				struct tm m_local_time;
				double m_msecs;
				int m_delta;
				void m_add(double dt)
				{
					m_msecs += dt;
					if (m_msecs > 1)
					{
						m_msecs -= 1;
						m_server_time.tm_sec += 1;
						m_local_time.tm_sec += 1;
						mktime(&m_server_time);
						mktime(&m_local_time);
					};
				};
			};

			tm getServerUTCTime()
			{
				return m_time.m_server_time;
			}

			tm getServerLocalTime()
			{
				return m_time.m_local_time;
			}

			void setTime(std::string s_iso8601, bool serverTimeValid)
			{
				tm localtm = getDeviceLocalTime();
				tm UTCtm = getDeviceUTCTime();
				time_t localtime_t = mktime(&localtm);
				time_t UTCtime_t = mktime(&UTCtm);
				if (serverTimeValid)
				{
					char* pos;
					m_time.m_server_time.tm_year = strtoul(s_iso8601.c_str(), &pos, 10) - 1900;
					m_time.m_server_time.tm_mon = strtoul(++pos, &pos, 10) - 1;
					m_time.m_server_time.tm_mday = strtoul(++pos, &pos, 10);
					m_time.m_server_time.tm_hour = strtoul(++pos, &pos, 10);
					m_time.m_server_time.tm_min = strtoul(++pos, &pos, 10);
					m_time.m_server_time.tm_sec = strtoul(++pos, &pos, 10);
					m_time.m_msecs = double(strtoul(++pos, &pos, 10)) / 1000;

					m_time.m_delta = (int)difftime(localtime_t, UTCtime_t);
					m_time.m_local_time = m_time.m_server_time;
					m_time.m_local_time.tm_sec += m_time.m_delta;
					mktime(&m_time.m_local_time);
				}
				else
				{
					m_time.m_server_time = UTCtm;
					m_time.m_local_time = localtm;
				}

			}


			///Get Current Time
			static struct tm getDeviceLocalTime();

			///Get Current Time
			static struct tm getDeviceUTCTime();

			/**
			* to receive timer initialized event
			* this will setup the shop
			*/
			void receive(WkCocos::Timer::Events::TimerInit const & ti);

		protected:

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			std::function<void()> m_error_callback;

			advanced_time m_time;

		};
	}// namespace Timer
}//namespace WkCocos
#endif //__WKCOCOS_TIMER_TIMER_H__