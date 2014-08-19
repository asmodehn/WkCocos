#ifndef __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__
#define __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Events
		{
			struct TimerUpdate : public entityx::Event<TimerUpdate>
			{
				TimerUpdate(entityx::Entity e, tm time) : timer(e), time_left(time)
				{
					auto tempEntity = e.component<Comp::ID>();
					id = tempEntity->m_id;
				}

				tm getTimeLeft() const
				{
					return time_left;
				}

				entityx::Entity timer;
				std::string		id;
				tm				time_left;
			};

		}//namespace Events
	}//namespace Timer
}//namespace WkCocos


#endif // __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__
