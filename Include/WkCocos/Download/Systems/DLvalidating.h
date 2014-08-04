#ifndef __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLVALIDATING_H__
#define __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLVALIDATING_H__

#include "cocos2d.h"

#include "entityx/entityx.h"

#include "WkCocos/Download/Comp/FileMD5.h"
#include "WkCocos/Download/Comp/CurlDL.h"

namespace WkCocos
{
	namespace Download
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
	}//namespace Download
}//namespace WkCocos

#endif // __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLVALIDATING_H__
