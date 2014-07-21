#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_LOADING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_LOADING_H__

#include "entityx/entityx.h"
#include "WkCocos/Loading/Comp/DataLoad.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			//system managing sync loading. one at a time.
			struct SyncLoading : public entityx::System<SyncLoading>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			};

			//system managing async loading. max_concurrent at a time.
			struct ASyncLoading : public entityx::System<ASyncLoading>
			{
				ASyncLoading(unsigned short max_concurrent);

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				unsigned short m_max_concurrent;
				unsigned short m_cur_concurrent;
			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_LOADING_H__
