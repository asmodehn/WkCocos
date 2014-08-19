#ifndef __WKCOCOS_TIMER_SYSTEMS_ALARMDESTRUCTOR_H__
#define __WKCOCOS_TIMER_SYSTEMS_ALARMDESTRUCTOR_H__

//#include "cocos2d.h"
#include "WkCocos/Utils/ToolBox.h"
#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct AlarmDestructor : public entityx::System<AlarmDestructor> {
				AlarmDestructor() {
				}
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos

#endif // __WKCOCOS_TIMER_SYSTEMS_ALARMDESTRUCTOR_H__
