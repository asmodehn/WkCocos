#ifndef __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__
#define __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__

//#include "cocos2d.h"

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct DataEval : public entityx::System<DataEval>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			
			protected:
				void chooseLoader(entityx::Entity entity, std::string datafile, bool force, entityx::ptr<entityx::EventManager> events);
			};

		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

#endif // __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__
