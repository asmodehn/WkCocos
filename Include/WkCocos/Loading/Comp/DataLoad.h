#ifndef __DFGAME_LOADING_ENTITY_COMP_DATALOAD_H__
#define __DFGAME_LOADING_ENTITY_COMP_DATALOAD_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Comp
		{
			struct DataListDownload : entityx::Component<DataListDownload> {
				DataListDownload(std::string url, unsigned long current_version, std::string minAppVersion, unsigned short retries = 3)
				: m_url(url)
				, m_current_version(current_version)
				, m_current_minAppVersion(minAppVersion)
				, m_retries(retries)
				{}

				unsigned long m_current_version;
				std::string m_current_minAppVersion;
				std::string m_url;
				unsigned short m_retries;
			};

			struct DataVerCheck : entityx::Component<DataVerCheck> {
				DataVerCheck(std::string url, long current_version, std::string minAppVersion, std::vector<std::string> verlist, unsigned short retries = 3)
				: m_url(url)
				, m_current_version(current_version)
				, m_current_minAppVersion(minAppVersion)
				, m_verlist(verlist)
				, m_retries(retries)
				{}

				std::vector<std::string> m_verlist;
				unsigned long m_current_version;
				std::string m_current_minAppVersion;
				std::string m_url;
				unsigned short m_retries;
			};
			
			struct DataLoad : entityx::Component<DataLoad> {
				DataLoad(std::string filepath) : m_filepath(filepath) {}

				inline std::string getFilepath() const
				{
					return m_filepath;
				}
				std::string m_filepath;
			};

		}//namespace Comp
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_COMP_DATALOAD_H__
