#include "WkCocos/Download/Systems/DLClisting.h"
#include "WkCocos/Download/Events/Error.h"

#include "WkCocos/Download/Comp/DataLoad.h"
#include "WkCocos/Download/Comp/ProgressValue.h"

#include "cocos2d.h"

#include <curl/curl.h>
#include <curl/easy.h>

//#include <regex>

namespace WkCocos
{
	namespace Download
	{
		namespace Systems
		{
			static size_t write_data(void *ptr, size_t size, size_t nmemb, void *userdata)
			{
				std::string * dirlist = (std::string*)userdata;
				dirlist->append((char*)ptr, size * nmemb);

				return (size * nmemb);
			}

			DLClisting::DLClisting()
			try
			: _curl(nullptr)
			, _connectionTimeout(0)
			{
				CCLOG("DLClisting Curl Init");
				_curl = curl_easy_init();
				if (!_curl)
				{
					throw std::logic_error("CANNOT INIT CURL");
				}
				CCLOG("DLClisting Curl Init OK !");
			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start DLC listing : %s",e.what());
			}

			DLClisting::~DLClisting()
			{
				CCLOG("DLClisting Curl Cleanup");
				curl_easy_cleanup(_curl);
				CCLOG("DLClisting Curl Cleanup OK !");
			}

			void DLClisting::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				for (auto entity : es->entities_with_components<Comp::DataListDownload>())
				{
					entityx::ptr<Comp::DataListDownload> dllist = entity.component<Comp::DataListDownload>();
					std::string url = dllist->m_url;
					//we are supposed to get a directory

					CCLOG("DLClisting reading from %s", url.c_str());


					//list all files at given url
					CURLcode res;
					curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
#ifdef _DEBUG
					curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
					curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
					curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &m_dirlist);
					if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
					curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
					curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
					curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
					CCLOG("CURL EASY PERFORM begin");
					res = curl_easy_perform(_curl);
					CCLOG("CURL EASY PERFORM end");

					CCLOG("DLClisting read from %s", url.c_str());

					if (res != 0)
					{

						CCLOG("DLClisting can not read from %s, error code is %d", url.c_str(), res);
						
						dllist->m_retries--;
						if (0 == dllist->m_retries)
						{
							CCLOGERROR("DLClisting can not read from %s, error code is %d", url.c_str(), res);
							//signal error
							events->emit<Events::Error>(entity);
						}
					}
					else
					{
						// extract what we need from the listing
						std::vector<std::string> m_version_vec;

						CCLOG("DLClisting before versions retrieval");
						//REGEX IMPLEMENTATION GCC 4.9 required
						//std::smatch m;
						////http://rextester.com/tester
						//std::regex e("<a href=\"(v[^ ]*/)\">");
						//std::string s = m_dirlist;
						//
						//CCLOG("DLClisting next string regex search : %s", s.c_str());
						//while (std::regex_search(s, m, e)) {
						//	if (m.size() > 0)
						//	{
						//		std::string verurl = m[1].str();
						//		CCLOG("DLClisting version found : %s", verurl.c_str());
						//		//if the version is superior ( or the same ! for client side fixes...)
						//		if (verurl >= dllist->m_current_version)
						//		{
						//			m_version_vec.push_back(verurl);
						//		}
						//	}
						//
						//	s = m.suffix().str();
						//	CCLOG("DLClisting next string regex search : %s", s.c_str());
						//}
						
						//STRING FIND IMPLEMENTATION
						std::string s = m_dirlist;
						std::string atag = "<a href=\"v";
						size_t pos = s.find(atag);
						while (pos != std::string::npos) //occurence found
						{
							pos = pos + atag.length()-1; //we want the 'v' in it
							size_t endpos = s.find("\">", pos);
							if (endpos != std::string::npos) //occurence found
							{
								std::string verstr = s.substr(pos , endpos - pos );
								CCLOG("verstr = %s ", verstr.c_str());
								m_version_vec.push_back(verstr);
							}
							pos = s.find(atag, endpos);
						}

						entity.remove<Comp::DataListDownload>();
						entity.assign<Comp::DataVerCheck>(dllist->m_url, dllist->m_current_version, dllist->m_current_minAppVersion, m_version_vec);

						CCLOG("DLClisting after versions retrieval");

						//exit this loop. one per update is enough
						break;
					}
				}
			}

			void DLClisting::setConnectionTimeout(unsigned int timeout)
			{
				_connectionTimeout = timeout;
			}
					

					
		}//namespace Systems
	}//namespace Download
}//namespace WkCocos

