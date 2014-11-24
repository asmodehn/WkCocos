#ifndef __WKCOCOS_DOWNLOAD_EVENTS_DOWNLOADADVISED_H__
#define __WKCOCOS_DOWNLOAD_EVENTS_DOWNLOADADVISED_H__

#include "entityx/entityx.h"
#include "WkCocos/Download/Version.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
            struct DownloadAdvised : public entityx::Event<DownloadAdvised>
            {
                DownloadAdvised( std::string url, Version version, std::string manifest_path, bool started)
                : m_url(url)
                , m_version(version)
                , m_manifest_path(manifest_path)
                , m_started(started)
                {
                }

                std::string m_url;
                Version m_version;
                std::string m_manifest_path;
                bool m_started;
            };

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __WKCOCOS_DOWNLOAD_EVENTS_DOWNLOADADVISED_H__
