#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DATAEVAL_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DATAEVAL_H__

#include "cocos2d.h"

#include "entityx/entityx.h"
#include "dfgame/Loading/Entity/Comp/DataLoad.h"
#include "dfgame/Loading/Entity/Comp/LoadFunc.h"

#include "dfgame/UI/Interface.h"

namespace dfgame
{
	namespace Loading
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct DataEval : public entityx::System<DataEval> {

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			protected:
				void chooseLoader(entityx::Entity entity, std::string respath, entityx::ptr<entityx::EventManager> events);
			};

		}//namespace Systems
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_LOADING_H__
