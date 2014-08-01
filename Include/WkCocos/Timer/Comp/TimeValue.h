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
				Alarm(struct tm end)
				: m_end(end)
				{}

				struct tm m_end;
			};


			struct Callback : entityx::Component<Callback>
			{
				Callback(std::function<void(std::string id, unsigned long msecs_elapsed)> cb)
				: m_cb(cb)
				{}

				std::function<void(std::string id, unsigned long msecs_elapsed)> m_cb;
			};

		}//namespace Comp
	}//namespace Timer
}//namespace WkCocos

#endif // __WKCOCOS_TIMER_TIMEVALUE_H__
