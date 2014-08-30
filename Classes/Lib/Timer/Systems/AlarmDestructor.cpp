#include "WkCocos/Timer/Systems/AlarmDestructor.h"
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
			void AlarmDestructor::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Stopped> stopped;
				for (auto entity : es->entities_with_components(id, stopped))
				{
					entity.destroy();
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


