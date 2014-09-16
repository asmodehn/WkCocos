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
				TimerUpdate(entityx::Entity e, double timeLeft) : timer(e), time_left(timeLeft)
				{
					auto tempEntity = e.component<Comp::ID>();
					id = tempEntity->m_id;
				}

				int getTimeLeft() const
				{
					return (int)time_left;
				}

				entityx::Entity timer;
				std::string		id;
				double			time_left;
			};

		}//namespace Events
	}//namespace Timer
}//namespace WkCocos


#endif // __WKCOCOS_TIMER_EVENTS_TIMERUPDATE_H__
