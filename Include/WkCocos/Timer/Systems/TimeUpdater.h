#ifndef __WKCOCOS_TIMER_SYSTEMS_TIMEUPDATER_H__
#define __WKCOCOS_TIMER_SYSTEMS_TIMEUPDATER_H__

//#include "cocos2d.h"
//#include "WkCocos/Timer/Timer.h"
#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct TimeUpdater : public entityx::System<TimeUpdater> {
				TimeUpdater() : oldtime(0){
				}
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
				time_t oldtime;
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos

#endif // __WKCOCOS_TIMER_SYSTEMS_TIMEUPDATER_H__
