#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DLCCHECKING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DLCCHECKING_H__

#include "entityx/entityx.h"

#include "WkCocos/Loading/Comp/ProgressValue.h"

#define LOW_SPEED_LIMIT 30L
#define LOW_SPEED_TIME 5L

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			//system managing checking which files to download from DLC.
			struct DLCchecking : public entityx::System<DLCchecking>
			{
				DLCchecking();

				~DLCchecking();

				std::vector<unsigned long> splitVersion(std::string vstr);

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				void setConnectionTimeout(unsigned int timeout);

				void *_curl;
				unsigned int _connectionTimeout;

				std::string m_manifest;

			};
			
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_DLCCHECKING_H__
