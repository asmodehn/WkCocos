#ifndef __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"
#include "WkCocos/Download/Comp/CurlDL.h"
#include "WkCocos/Download/Comp/DataLoad.h"
#include "WkCocos/Download/Comp/FileMD5.h"

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

					auto tempLF = e.component<Comp::LocalFile>();
					auto tempTF = e.component<Comp::TempFile>();
					auto tempTS = e.component<Comp::TempMD5>();
					auto tempRF = e.component<Comp::RemoteFile>();
					auto tempRS = e.component<Comp::RemoteMD5>();
					if (tempLF && tempTF && tempTS && tempRF && tempRS)
					{
						msg = system_msg + ", downloaded file signature " + tempTS->getMD5() + " doesnt match expected signature : " + tempRS->getMD5();
					}
				}

				entityx::Entity errored_entity;

				std::string msg;

			};

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
