#ifndef __WKCOCOS_TIMER_EVENTS_ALARMOFF_H__
#define __WKCOCOS_TIMER_EVENTS_ALARMOFF_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Events
		{
			struct AlarmOff : public entityx::Event<AlarmOff> {
				AlarmOff(entityx::Entity e, const std::string& timerID)
				: alarmoff_entity(e)
				, id(timerID)
				{}

				entityx::Entity alarmoff_entity;
				std::string		id;
			};

		}//namespace Events
	}//namespace Timer
}//namespace WkCocos


#endif // __WKCOCOS_TIMER_EVENTS_ALARMOFF_H__
