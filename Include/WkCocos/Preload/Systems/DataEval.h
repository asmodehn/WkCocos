#ifndef __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__
#define __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__

#include "cocos2d.h"

#include "entityx/entityx.h"
#include "WkCocos/Preload/Comp/DataLoad.h"
#include "WkCocos/Preload/Events/Downloaded.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct DataEval : public entityx::System<DataEval>, entityx::Receiver<DataEval>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
				void receive(const Events::Downloaded &dl);
			
			protected:
				void chooseLoader(entityx::Entity entity, std::string datafile, entityx::ptr<entityx::EventManager> events);
			};

		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

#endif // __WKCOCOS_PRELOAD_ENTITY_SYSTEMS_DATAEVAL_H__
