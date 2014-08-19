#ifndef __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"
#include "WkCocos/Download/Comp/CurlDL.h"
#include "WkCocos/Download/Comp/DataLoad.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error>
			{
				Error(entityx::Entity e, std::string system_msg) : errored_entity(e)
				{
					auto tempCDL = e.component<Comp::CurlDL>();
					if (tempCDL)
					{
						
						msg = system_msg + ", " + tempCDL->getErrorMsg();
					}

					auto tempDVC = e.component<Comp::DataVerCheck>();
					if (tempDVC)
					{
						msg = system_msg + ", can not read from " + tempDVC->m_url;
					}

					auto tempDLD = e.component<Comp::DataListDownload>();
					if (tempDLD)
					{
						msg = system_msg + ", can not read from " + tempDLD->m_url;
					}
				}

				entityx::Entity errored_entity;

				std::string msg;

			};

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
