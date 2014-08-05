#ifndef __DFGAME_PRELOAD_ENTITY_SYSTEMS_SYNCLOADING_H__
#define __DFGAME_PRELOAD_ENTITY_SYSTEMS_SYNCLOADING_H__

#include "entityx/entityx.h"
#include "WkCocos/Preload/Comp/DataLoad.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			//system managing sync loading. one at a time.
			struct SyncLoading : public entityx::System<SyncLoading>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			};

		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_SYSTEMS_SYNCLOADING_H__
