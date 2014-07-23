#ifndef __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCCHECKING_H__
#define __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCCHECKING_H__

#include "entityx/entityx.h"

#include "WkCocos/Download/Comp/ProgressValue.h"

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

namespace WkCocos
{
	namespace Download
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
	}//namespace Download
}//namespace WkCocos

#endif // __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCCHECKING_H__
