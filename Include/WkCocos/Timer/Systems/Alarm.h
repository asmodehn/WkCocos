#ifndef __WKCOCOS_TIMER_SYSTEMS_ALARM_H__
#define __WKCOCOS_TIMER_SYSTEMS_ALARM_H__

//#include "cocos2d.h"
//#include "WkCocos/Utils/ToolBox.h"
#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct Alarm : public entityx::System<Alarm> {
				// does not compile on android, Error:(17, 53) error: taking address of temporary [-fpermissive]
				//Alarm() : oldtime(mktime(&ToolBox::getUTCTime())) {}
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
				//time_t oldtime;
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos

#endif // __WKCOCOS_TIMER_SYSTEMS_ALARM_H__
