#ifndef __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCLISTING_H__
#define __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCLISTING_H__

#include "entityx/entityx.h"

#define LOW_SPEED_LIMIT 30L
#define LOW_SPEED_TIME 5L

namespace WkCocos
{
	namespace Download
	{
		namespace Systems
		{
			//system getting the list of DLC versions available.
			struct DLClisting : public entityx::System<DLClisting>
			{
				DLClisting();

				~DLClisting();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				void setConnectionTimeout(unsigned int timeout);

				void *_curl;
				unsigned int _connectionTimeout;

				std::string m_dirlist;

			};
			
		}//namespace Systems
	}//namespace Download
}//namespace WkCocos

#endif // __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_DLCLISTING_H__
