#include "WkCocos/Loading/Systems/DLCchecking.h"
#include "WkCocos/Loading/Events/Error.h"

#include "WkCocos/Loading/Comp/DataLoad.h"
#include "WkCocos/Loading/Comp/ProgressValue.h"

#include "WkCocos/Utils/ToolBox.h"

#include "WkCocos/Utils/Crypto.h"

#include "cocostudio/CocoStudio.h"

#include <curl/curl.h>
#include <curl/easy.h>

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			static size_t write_data(void *ptr, size_t size, size_t nmemb, void *userdata)
			{
				std::string * dirlist = (std::string*)userdata;
				dirlist->append((char*)ptr, size * nmemb);

				return (size * nmemb);
			}

			static size_t download_file(void *ptr, size_t size, size_t nmemb, void *userdata)
			{
				FILE *fp = (FILE*)userdata;
				size_t written = fwrite(ptr, size, nmemb, fp);
				return written;
			}

			static int download_progress(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
			{
				static int percent = 0;
				int tmp = (int)(nowDownloaded / totalToDownload * 100);

				if (percent != tmp)
				{
					percent = tmp;
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
					//	auto manager = static_cast<AssetsManager*>(ptr);
					//	if (manager->_delegate)
					//		manager->_delegate->onProgress(percent);
					//});

					CCLOG("downloading... %d%%", percent);
				}

				return 0;
			}

			DLCchecking::DLCchecking()
			try
			: _curl(nullptr)
			, _connectionTimeout(0)
			, _retries(3)
			{
				CCLOG("DLCchecking Curl Init");
				_curl = curl_easy_init();
				if (!_curl)
				{
					throw std::logic_error("CANNOT INIT CURL");
				}
				CCLOG("DLCchecking Curl Init OK !");
			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start DLC checking : %s",e.what());
			}

			DLCchecking::~DLCchecking()
			{
				curl_easy_cleanup(_curl);
			}

			std::vector<unsigned long> DLCchecking::splitVersion(std::string vstr)
			{
				std::vector<unsigned long> sv;
				std::string delimiters = ".";
				size_t current;
				size_t next = -1;
				do
				{
					current = next + 1;
					next = vstr.find_first_of(delimiters, current);
					std::cout << vstr.substr(current, next - current) << std::endl;
					unsigned long tmp = 0;
					try
					{
						tmp = ToolBox::stoul(vstr.substr(current, next - current));
					}
					catch (std::out_of_range oor)
					{
						tmp = LONG_MAX;
					}
					sv.push_back(tmp);
				} while (next != std::string::npos);
				return sv;
			}


			void DLCchecking::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				for (auto entity : es->entities_with_components<Comp::DataVerCheck>())
				{
					entityx::ptr<Comp::DataVerCheck> dllist = entity.component<Comp::DataVerCheck>();
					std::string url = dllist->m_url;

					if (!dllist->m_verlist.empty())
					{
						//we need to check if the version url is valid ( contains a manifest.json )
						url += "/" + dllist->m_verlist.back() + "/manifest.json";
						m_manifest = "";
						CCLOG("DLCchecking reading from %s", url.c_str());

						//list all files at given url
						CURLcode res;
						curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
#ifdef _DEBUG
						curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
						curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
						curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &m_manifest);
						if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
						curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
						curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
						curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
						res = curl_easy_perform(_curl);

						CCLOG("DLCchecking read from %s", url.c_str());

						if (res != 0)
						{

							CCLOG("DLCchecking can not read from %s, error code is %d", url.c_str(), res);

							dllist->m_retries--;
							if (0 == dllist->m_retries)
							{
								CCLOGERROR("DLCchecking can not read from %s, error code is %d", url.c_str(), res);
								//signal error
								events->emit<Events::Error>(entity);
								//we give up on this entity
								entity.destroy();
							}
						}
						else
						{
							//read the downloaded manifest to compare md5
							rapidjson::Document json;
							json.Parse<0>(m_manifest.c_str());
							if (json.HasParseError()) {
								CCLOG("GetParseError %s\n", json.GetParseError());
								//version will be removed from the list later.
							}
							else
							{
								bool dlc_update_allowed = false;
								unsigned long version = 0;
								try{
									version = ToolBox::stoul(cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "version", 0));
								}
								catch (std::out_of_range oor)
								{
									version = LONG_MAX;
								}

								std::string minAppVersion = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "minAppVersion", "error");
								if (minAppVersion != "error" && dllist->m_current_minAppVersion != "" )
								{
									//CAREFUL HERE with version comparison
									std::vector<unsigned long> mav = splitVersion(minAppVersion);
									std::vector<unsigned long> cmav = splitVersion(dllist->m_current_minAppVersion);


									for (unsigned int i = 0; i < cmav.size(); ++i)
									{
										if (mav.at(i) <= cmav.at(i))
										{
											dlc_update_allowed = true;
										}
									}
								}//if we cannot read the minimum app version. we dont download. better safe than sorry.

								if (dlc_update_allowed && version >= dllist->m_current_version)
								{

									//prepare the list of downloads
									const rapidjson::Value & assets = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(json, "assets");

									for (rapidjson::Value::ConstMemberIterator m = assets.MemberonBegin(); m != assets.MemberonEnd(); ++m)
									{
										std::string filename = m->name.GetString();
										std::string filehash = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(m->value, "md5", "error");

										//lowering filehash to be sure
										std::transform(filehash.begin(), filehash.end(), filehash.begin(), ::tolower);

										//std::cout << filename << " : " << filehash << std::endl;

										entityx::Entity newentity = es->create();
										newentity.assign<Comp::DataDownload>(dllist->m_url + "/" + dllist->m_verlist.back(), filename, filehash);
										newentity.assign<Comp::ProgressValue>(1);
									}


									//downloading only the last verison should always be enough ( avoiding too many downloads - keeping all data for one version in same place )
									//if (dllist->m_verlist.empty()) //if we checked all versions
									//{
									entity.remove<Comp::DataVerCheck>();
									//}

									//we dont need to do anything more with this entity
									entity.destroy();
								}

							}

							//remove the version checked from the list
							dllist->m_verlist.pop_back();
							//In case of error, we should check the next version in stack on next update.
							//If success this will not be done ( entity destroyed )
							//TODO : check behavior

						}

						//exit this loop. one per update is enough
						break;
			
					}
					else //no version left to check
					{
						//we dont need to do anything more with this entity
						entity.destroy();
					}
				}
			};

			void DLCchecking::setConnectionTimeout(unsigned int timeout)
			{
				_connectionTimeout = timeout;
			}

					
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

