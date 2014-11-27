#ifndef __DOWNLOAD_ENTITY_DOWNLOAD_H__
#define __DOWNLOAD_ENTITY_DOWNLOAD_H__

#include "cocos2d.h"

#include "entityx/entityx.h"

#include "WkCocos/Download/Events/DownloadOptions.h"
#include "WkCocos/Download/Events/DownloadAdvised.h"

#include "curl/curl.h"

namespace WkCocos
{
	namespace Download
	{
	    //TODO : This mix the feature of an actor ( implement a high level feature such as : "stay up to date" )
	    // and a manager for entity systems.
	    // These should be split in two parts. The manager does lower entity system management ( and underneath events )
	    // The Actor triggers high level events related with the feature it is implementing.
		class Download
			{
			public:
				explicit Download(unsigned short concurrent_downloads,
					std::function<void(float)> progress_callback // THIS WILL BE REMOVED SOON!!!
					);

				//TODO: This should return true if DLC will be done, or false if the manifest disables DLC.
				void addDataDownload(Version currentVersion,const std::string json_manifest_filename);

				virtual ~Download();

				entityx::ptr<entityx::EventManager> getEventManager()
				{
					return event_manager;
				}

				entityx::ptr<entityx::SystemManager> getSystemManager()
				{
					return system_manager;
				}

				void update(double dt);

				/**
				* returning the current download progress
				* @return pair where first is the current progress, and second is the total amount to get to.
				*/
				std::pair<int,int> getCurrentProgress();


			protected:

				unsigned short m_concurrent_downloads;

				std::function<void(float)> m_progress_callback;

              entityx::ptr<entityx::EventManager> event_manager;
              entityx::ptr<entityx::EntityManager> entity_manager;
              entityx::ptr<entityx::SystemManager> system_manager;

			};

	} // namespace Download
}  // namespace WkCocos

#endif //__DOWNLOAD_ENTITY_DOWNLOAD_H__
