#ifndef __WKCOCOS_TIMER_TIMER_H__
#define __WKCOCOS_TIMER_TIMER_H__

#include "entityx/entityx.h"

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
			* @param msecs duration of the timer
			* @param update_cb function pointer called everyupdate with the current time difference passed in msec_elapsed
			*/
			bool setTimer(std::string id, unsigned long msecs, std::function<void(std::string id, unsigned long msecs_elapsed)> update_cb);

			/**
			* StartTimer
			*/
			bool startTimer(std::string id);
			
			/**
			* Stop Timer (Doesnt trigger callback)
			*/
			void stopTimer(std::string id);

			/**
			* Delete Timer
			*/
			void deleteTimer(std::string id);

			///Get Current Time
			struct tm getLocalTime();
			///Get Current Time
			struct tm getUTCTime();

			void update(double dt);

		protected:

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			std::function<void()> m_error_callback;


		};
	}// namespace Timer
}//namespace WkCocos
#endif //__WKCOCOS_TIMER_TIMER_H__