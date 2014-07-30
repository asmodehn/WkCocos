#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DLVALIDATING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DLVALIDATING_H__

//#include "cocos2d.h"

#include "entityx/entityx.h"
#include "WkCocos/Loading/Comp/DataLoad.h"
#include "WkCocos/Loading/Comp/FileMD5.h"
#include "WkCocos/Loading/Comp/CurlDL.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			/**
			* This System checks if an entity has a LocalFile, a TempFile, a remoteMD5 and a TempMD5 component.
			* If it is the case, and if TempMD5 matches remoteMD5, it will validate download by moving the TempFile to LocalFile.
			**/
			struct DLvalidating : public entityx::System<DLvalidating>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			protected:

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_DLVALIDATING_H__
