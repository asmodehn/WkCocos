#include "WkCocos/Preload/Systems/SyncLoading.h"

#include "WkCocos/Preload/Comp/LoadFunc.h"
#include "WkCocos/Preload/Comp/ProgressValue.h"

#include "WkCocos/Preload/Events/Error.h"
#include "WkCocos/Preload/Events/Loaded.h"

#include "cocos/cocos2d.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			void SyncLoading::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				unsigned short minprio = Comp::LoadSyncFunc::load_priority_max;
				entityx::Entity loadentity;
				for (auto entity : es->entities_with_components<Comp::LoadSyncFunc>())
				{
					//selection based on priority
					entityx::ptr<Comp::LoadSyncFunc> loadfunc = entity.component<Comp::LoadSyncFunc>();
					if (loadfunc->getPriority() < minprio)
					{
						minprio = loadfunc->getPriority();
						loadentity = entity;
					}
				}

				//we need to test loadentity is valid ( default constructor builds invalid entity )
				if (loadentity)
				{
					entityx::ptr<Comp::LoadSyncFunc> loadfunc = loadentity.component<Comp::LoadSyncFunc>();
					loadfunc->getLoadfunction();
					//signal loaded
					events->emit<Events::Loaded>(loadfunc->getFilepath());
					//register progress
					loadentity.remove<Comp::ProgressValue>();
				}
			}

		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

