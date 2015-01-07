#include "WkCocos/Timer/Systems/TimeUpdater.h"
#include "WkCocos/Timer/Events/AlarmOff.h"
#include "WkCocos/Timer/Events/TimerUpdate.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			/**
			* This function will call the callback with the current progress of the timer.
			*/
			void TimeUpdater::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Alarm> alarm;

				for (auto entity : es->entities_with_components(id, alarm))
				{
					alarm->m_msecs += dt;
					if (alarm->m_msecs > 1.0)
					{
						alarm->m_msecs -= 1.0;
						alarm->m_now.tm_sec += 1;

						time_t now = mktime(&alarm->m_now);
						time_t end = mktime(&alarm->m_end);
						double delta = difftime(end, now);

						events->emit<Events::TimerUpdate>(entity, delta);
					}
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


