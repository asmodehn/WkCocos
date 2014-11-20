#ifndef __DFGAME_DOWNLOAD_ENTITY_COMP_DATALOAD_H__
#define __DFGAME_DOWNLOAD_ENTITY_COMP_DATALOAD_H__

#include "entityx/entityx.h"
#include "WkCocos/Utils/ToolBox.h"

namespace WkCocos
{
	namespace Download
	{

	    struct Version
	    {
	        Version(std::string vstr)
	        : m_version_str(vstr)
	        {
	            if (m_version_str[0] != 'v')
                {
                    throw std::logic_error("This string is not a version string !");
                }
	        }

	        Version(std::vector<unsigned long> version)
	        : m_version_str("v")
			{
				for (unsigned long num : version)
				{
					m_version_str += WkCocos::ToolBox::to_string(num) + ".";
				}
				m_version_str.pop_back(); // removing trailing '.'
			}

	        std::vector<unsigned long> split()
	        {
	            std::vector<unsigned long> sv;

				if (m_version_str.length() > 0)
				{
					if (m_version_str.at(0) == 'v')
					{
						//removing first 'v'
						m_version_str = m_version_str.substr(1);
					}

					std::string delimiters = ".";
					size_t current;
					size_t next = -1;
					do
					{
						current = next + 1;
						next = m_version_str.find_first_of(delimiters, current);
						//in case we do not find any delimiter
						size_t endsub = (std::string::npos == next) ? next : next - current;
						//std::cout << m_version_str.substr(current, next - current) << std::endl;
						unsigned long tmp = 0;
						try
						{
							tmp = ToolBox::stoul(m_version_str.substr(current, endsub));
						}
						catch (std::out_of_range oor)
						{
							tmp = ULONG_MAX;
						}
						sv.push_back(tmp);
					} while (next != std::string::npos);
				}
				return sv;
	        }

            bool operator ==(Version const& v)
            {
                Version vcopy = v;
                std::vector<unsigned long> v1l = split();
				std::vector<unsigned long> v2l = vcopy.split();
				bool equals = true;
				//getting all version to same size by assuming missing one is 0.
				while (v1l.size() < v2l.size())
				{
					v1l.push_back(0);
				}
				while (v1l.size() > v2l.size())
				{
					v2l.push_back(0);
				}

				for (unsigned int i = 0; i < v1l.size(); ++i)
				{
					if (v1l.at(i) != v2l.at(i))
					{
						equals = false; break;
					}
				}
				return equals;
            }
            bool operator!=(Version const& v){return !(m_version_str == v.m_version_str);}

            bool operator< (Version const& v)
            {
                Version vcopy = v;
                std::vector<unsigned long> v1l = split();
				std::vector<unsigned long> v2l = vcopy.split();
				bool more = true;
				//getting all version to same size by assuming missing one is 0.
				while (v1l.size() < v2l.size())
				{
					v1l.push_back(0);
				}
				while (v1l.size() > v2l.size())
				{
					v2l.push_back(0);
				}

				for (unsigned int i = 0; i < v1l.size(); ++i)
				{
					if (v1l.at(i) < v2l.at(i))
					{
						more = false; break;// if left number is less, we don't need to check right hand numbers. this version is less.
					}
				}
#ifdef _DEBUG
				if (more)
				{
					std::cout << "Version " << m_version_str << " >= " << v.m_version_str << std::endl;
				}
				else
				{
					std::cout << "Version " << m_version_str << " < " << v.m_version_str << std::endl;
				}
#endif
				return !more;
            }
            bool operator> (Version const& v){return v.m_version_str < m_version_str;}
            bool operator<=(Version const& v){return !(m_version_str > v.m_version_str);}
            bool operator>=(Version const& v){return !(m_version_str < v.m_version_str);}

	        std::string m_version_str;
	    };

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
