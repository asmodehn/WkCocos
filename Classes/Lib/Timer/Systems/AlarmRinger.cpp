#include "WkCocos/Timer/Systems/AlarmRinger.h"
#include "WkCocos/Timer/Comp/TimeValue.h"
#include "WkCocos/Timer/Events/AlarmOff.h"
#include "WkCocos/Timer/Events/TimerUpdate.h"
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
			void AlarmRinger::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Alarm> alarm;
				for (auto entity : es->entities_with_components(id, alarm))
				{
					struct tm now = ToolBox::getUTCTime();
					time_t start = mktime(&alarm->m_end);
					time_t nowtime = mktime(&now);
					// actually returns long long, despite double declaration
					double delta = difftime(start, nowtime); 
					if (delta < 0) // becomes true when delta is -1, this adds 1 sec to alarm length
					{
						events->emit<Events::AlarmOff>(entity);
					}
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


