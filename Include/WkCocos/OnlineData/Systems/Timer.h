#ifndef __DFGAME_ONLINEDATA_SYSTEMS_TIMER_H__
#define __DFGAME_ONLINEDATA_SYSTEMS_TIMER_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

namespace WkCocos 
{
	namespace OnlineData
	{
		namespace Systems
		{
			struct Timer : public entityx::System<Timer>
			{
				Timer();
				~Timer();
				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;

			protected :
				::App42::TimerService* m_timer_service;
			};
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

#endif // __DFGAME_ONLINEDATA_SYSTEMS_TIMER_H__
