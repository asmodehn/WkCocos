#ifndef __WKCOCOS_TIMER_EVENTS_TIMERINIT_H__
#define __WKCOCOS_TIMER_EVENTS_TIMERINIT_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Events
		{
			struct TimerInit : public entityx::Event<TimerInit>
			{
				TimerInit(){}

				virtual ~TimerInit(){}
			};

		}//namespace Events
	}//namespace Timer
}//namespace WkCocos


#endif // __WKCOCOS_TIMER_EVENTS_TIMERINIT_H__
