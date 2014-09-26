#ifndef __DFGAME_DOWNLOAD_ENTITY_COMP_DATALOAD_H__
#define __DFGAME_DOWNLOAD_ENTITY_COMP_DATALOAD_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Comp
		{
			struct DataListDownload : entityx::Component<DataListDownload> {
				DataListDownload(std::string url, std::string current_version, std::string minAppVersion, unsigned short retries = 3)
				: m_url(url)
				, m_current_version(current_version)
				, m_current_minAppVersion(minAppVersion)
				, m_retries(retries)
				{}

				std::string m_current_version;
				std::string m_current_minAppVersion;
				std::string m_url;
				unsigned short m_retries;
			};

			struct DataVerCheck : entityx::Component<DataVerCheck> {
				DataVerCheck(std::string url, std::string current_version, std::string minAppVersion, std::vector<std::string> verlist, unsigned short retries = 3)
				: m_url(url)
				, m_current_version(current_version)
				, m_current_minAppVersion(minAppVersion)
				, m_verlist(verlist)
				, m_retries(retries)
				{}

				std::vector<std::string> m_verlist;
				std::string m_current_version;
				std::string m_current_minAppVersion;
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

#endif // __DFGAME_DOWNLOAD_ENTITY_COMP_DATALOAD_H__
