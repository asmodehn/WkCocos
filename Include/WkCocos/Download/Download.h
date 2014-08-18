#ifndef __DOWNLOAD_ENTITY_DOWNLOAD_H__
#define __DOWNLOAD_ENTITY_DOWNLOAD_H__

#include "cocos2d.h"

#include "entityx/entityx.h"

#include "curl/curl.h"

namespace WkCocos 
{
	namespace Download
	{
		class Download : public entityx::Manager
			{
			public:
				explicit Download(unsigned short concurrent_downloads,
					std::function<void(float)> progress_callback
					);

				//the assetsManager must be added to a scene to have its update called
				void addDataDownload(const std::string json_manifest_filename);

				virtual ~Download();

				entityx::ptr<entityx::EventManager> getEventManager()
				{
					return event_manager;
				}

				entityx::ptr<entityx::SystemManager> getSystemManager()
				{
					return system_manager;
				}

			protected:
				void configure() override;

				void initialize() override;

				void update(double dt) override;

				unsigned short m_concurrent_downloads;

				std::function<void(float)> m_progress_callback;

				
			};

	} // namespace Download
}  // namespace WkCocos

#endif //__DOWNLOAD_ENTITY_DOWNLOAD_H__
