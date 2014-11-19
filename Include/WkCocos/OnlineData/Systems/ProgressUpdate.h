#ifndef __DFGAME_ONLINEDATA_SYSTEMS_PROGRESSUPDATE_H__
#define __DFGAME_ONLINEDATA_SYSTEMS_PROGRESSUPDATE_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

namespace WkCocos 
{
	namespace OnlineData
	{
		namespace Systems
		{
			struct ProgressUpdate : public entityx::System<ProgressUpdate>
			{
				ProgressUpdate();
				~ProgressUpdate();
				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;
			};
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

#endif // __DFGAME_ONLINEDATA_SYSTEMS_PROGRESSUPDATE_H__
