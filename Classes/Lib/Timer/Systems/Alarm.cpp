#include "WkCocos/Timer/Systems/Alarm.h"
#include "WkCocos/Timer/Comp/TimeValue.h"
#include "WkCocos/Timer/Events/AlarmOff.h"

#include "WkCocos/Utils/ToolBox.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			/**
			* This function will call the callback with the current progress of the timer.
			*/
			void Alarm::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Alarm> alarm;
				entityx::ptr<Comp::Callback> cb;
				for (auto entity : es->entities_with_components(id, alarm, cb))
				{
					struct tm now = ToolBox::getUTCTime();
					time_t start = mktime(&alarm->m_end);
					time_t nowtime = mktime(&now);
					double delta = difftime(start, nowtime );
					if (delta < 0)
					{
						events->emit<Events::AlarmOff>(entity);
						cb->m_cb(id->m_id, "alarm");
					}
					else
						cb->m_cb(id->m_id, ToolBox::itoa(delta));
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


