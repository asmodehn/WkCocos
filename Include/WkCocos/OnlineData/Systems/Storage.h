#ifndef __DFGAME_ONLINEDATA_SYSTEMS_STORAGE_H__
#define __DFGAME_ONLINEDATA_SYSTEMS_STORAGE_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

namespace WkCocos 
{
	namespace OnlineData
	{
		namespace Systems
		{
			//system finding error and launching error behavior
			struct Storage : public entityx::System<Storage>
			{
				Storage();
				~Storage();
				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;

			protected :
				::App42::StorageService* m_service;
			};
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

#endif // __DFGAME_ONLINEDATA_SYSTEMS_STORAGE_H__
