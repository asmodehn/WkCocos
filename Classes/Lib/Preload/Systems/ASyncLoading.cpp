#include "WkCocos/Preload/Systems/ASyncLoading.h"

#include "WkCocos/Preload/Comp/LoadFunc.h"
#include "WkCocos/Preload/Comp/ProgressValue.h"

#include "WkCocos/Preload/Events/Error.h"
#include "WkCocos/Preload/Events/Loaded.h"

//#include "cocos/cocos2d.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			ASyncLoading::ASyncLoading(unsigned short max_concurrent)
			: m_max_concurrent(max_concurrent), m_cur_concurrent(0)
			{
			}

			void ASyncLoading::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				//guard against update call while async function is still running.
				if (m_cur_concurrent < m_max_concurrent)
				{
					unsigned short minprio = Comp::LoadASyncFunc::load_priority_max;
					entityx::Entity loadentity;
					for (auto entity : es->entities_with_components<Comp::LoadASyncFunc>())
					{
						//selection based on priority
						entityx::ptr<Comp::LoadASyncFunc> loadfunc = entity.component<Comp::LoadASyncFunc>();
						if (false == loadfunc->getInprogress() && loadfunc->getPriority() < minprio)
						{
							minprio = loadfunc->getPriority();
							loadentity = entity;
						}
					}

					//make sure identity is valid ( default constructor builds invalid entity )
					if (loadentity)
					{
						entityx::ptr<Comp::LoadASyncFunc> loadfunc = loadentity.component<Comp::LoadASyncFunc>();
						m_cur_concurrent++;
						loadfunc->setInprogress(true);
						loadentity.remove<Comp::LoadASyncFunc>();

						//this is supposed to stay alive until all entities are gone.
						auto lf = loadfunc->getLoadfunction();
						lf ([this, events, loadfunc, loadentity](cocos2d::Texture2D* t2d){
							//WARNING : this should be run in same thread as the update to avoid any undesired side effects, like game crash.
							entityx::Entity copy = loadentity;
							//signal loaded
							CCLOG("=> Sending loaded event for %s", loadfunc->getFilepath().c_str());
							events->emit<Events::Loaded>(loadfunc->getFilepath());
							copy.remove<Comp::ProgressValue>();
							this->m_cur_concurrent--;
						});
					}
				}
			}

		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

