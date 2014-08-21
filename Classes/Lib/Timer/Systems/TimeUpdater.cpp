#include "WkCocos/Timer/Systems/TimeUpdater.h"
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
			void TimeUpdater::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Alarm> alarm;

				struct tm now = ToolBox::getUTCTime();
				time_t nowtime = mktime(&now);
				bool updateOldTime = false;

				for (auto entity : es->entities_with_components(id, alarm))
				{
					time_t start = mktime(&alarm->m_end);
					// actually returns long long, despite double declaration
					double delta = difftime(start, nowtime);
					if (difftime(nowtime, oldtime) > 0)
					{
						updateOldTime = true;
						tm temptime = { (time_t)delta, 0, 0, 0, 0, 0, 0, 0, 0 };
						mktime(&temptime);
						events->emit<Events::TimerUpdate>(entity, temptime, delta);
					}
				}

				if (updateOldTime)
				{
					oldtime = nowtime;
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


