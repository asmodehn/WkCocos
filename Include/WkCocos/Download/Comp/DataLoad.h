#ifndef __WKCOCOS_DOWNLOAD_ENTITY_COMP_DATALOAD_H__
#define __WKCOCOS_DOWNLOAD_ENTITY_COMP_DATALOAD_H__

#include "entityx/entityx.h"
#include "WkCocos/Utils/ToolBox.h"
#include "WkCocos/Download/Version.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Comp
		{
			struct DataListDownload : entityx::Component<DataListDownload> {
				DataListDownload(std::string url, Version current_dataVersion, Version appVersion, unsigned short retries = 3)
				: m_url(url)
				, m_current_dataVersion(current_dataVersion)
				, m_currentAppVersion(appVersion)
				, m_retries(retries)
				{}

				Version m_current_dataVersion;
				Version m_currentAppVersion;
				std::string m_url;
				unsigned short m_retries;
			};

			struct DataVerCheck : entityx::Component<DataVerCheck> {
				DataVerCheck(std::string url, Version current_dataVersion, Version appVersion, std::vector<Version> verlist, unsigned short retries = 3)
				: m_url(url)
				, m_current_dataVersion(current_dataVersion)
				, m_currentAppVersion(appVersion)
				, m_verlist(verlist)
				, m_retries(retries)
				{}

				std::vector<Version> m_verlist;
				Version m_current_dataVersion;
				Version m_currentAppVersion;
				std::string m_url;
				unsigned short m_retries;
			};

			struct DataDownload : entityx::Component<DataDownload> {
				DataDownload(std::string url, std::string filepath, std::string hash, unsigned short retries = 3) : m_url(url), m_filepath(filepath), m_hash(hash), m_retries(retries) {}

				std::string m_url;
				std::string m_filepath;
				std::string m_hash;
				unsigned short m_retries;
			};

		}//namespace Comp
	}//namespace Download
}//namespace WkCocos

#endif // __WKCOCOS_DOWNLOAD_ENTITY_COMP_DATALOAD_H__
