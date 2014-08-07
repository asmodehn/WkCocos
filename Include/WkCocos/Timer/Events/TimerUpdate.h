#ifndef __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__
#define __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Events
		{
			struct TimerUpdate : public entityx::Event<TimerUpdate> {
				TimerUpdate(entityx::Entity e, const std::string& timerID, double time) 
				: timer(e)
				, id(timerID)
				, time_left(time)
				{}

				entityx::Entity timer;
				std::string		id;
				double			time_left;
			};

		}//namespace Events
	}//namespace Timer
}//namespace WkCocos


#endif // __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__
