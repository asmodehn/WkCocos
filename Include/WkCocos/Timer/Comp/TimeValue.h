#ifndef __WKCOCOS_TIMER_TIMEVALUE_H__
#define __WKCOCOS_TIMER_TIMEVALUE_H__

#include "entityx/entityx.h"

#include <ctime>

namespace WkCocos
{
	namespace Timer
	{
		namespace Comp
		{
			struct ID : entityx::Component<ID>
			{
				ID(std::string id)
				: m_id(id)
				{}

				std::string m_id;
			};

			struct Alarm : entityx::Component<Alarm>
			{
				Alarm(struct tm end, struct tm now, double msecs)
				: m_end(end)
				, m_now(now)
				, m_msecs(msecs)
				{}
				struct tm m_end;
				struct tm m_now;
				double m_msecs;
			};

			struct Stopped : entityx::Component<Stopped>
			{
				Stopped(){}
			};

		}//namespace Comp
	}//namespace Timer
}//namespace WkCocos

#endif // __WKCOCOS_TIMER_TIMEVALUE_H__
