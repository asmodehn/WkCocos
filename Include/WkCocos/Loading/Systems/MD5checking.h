#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_MD5CHECKING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_MD5CHECKING_H__

#include "cocos2d.h"

#include "entityx/entityx.h"
#include "WkCocos/Loading/Comp/DataLoad.h"
#include "WkCocos/Loading/Comp/CurlDL.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			/**
			* ThisSystem checks if an entity has a [Local|Temp]File component without [Local|Temp]MD5 component.
			* When it finds one, it will create the [Local|Temp]MD5 component with the file signature in it.
			**/
			struct MD5checking : public entityx::System<MD5checking>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;
			protected:

				std::string computeMD5(std::string filepath);
			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_MD5CHECKING_H__
