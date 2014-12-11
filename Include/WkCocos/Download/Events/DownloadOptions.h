#ifndef __DFGAME_DOWNLOAD_EVENTS_DOWNLOADOPTIONS_H__
#define __DFGAME_DOWNLOAD_EVENTS_DOWNLOADOPTIONS_H__

#include "entityx/entityx.h"
#include "WkCocos/Download/Version.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
            struct DownloadOptions : public entityx::Event<DownloadOptions>
            {
                DownloadOptions(std::string url, Version current_version, std::vector<Version> version_vec , Version force_update )
                : m_url(url)
                , m_current_version(current_version)
                , m_version_vec(version_vec)
                , m_force_update(force_update)
                {
                }

                std::string m_url;
                Version m_current_version;
                std::vector<Version> m_version_vec;
                Version m_force_update;
            };

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_EVENTS_DOWNLOADOPTIONS_H__
