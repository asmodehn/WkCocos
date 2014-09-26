#include "WkCocos/Download/Systems/DLCchecking.h"
#include "WkCocos/Download/Events/Error.h"

#include "WkCocos/Download/Comp/DataLoad.h"
#include "WkCocos/Download/Comp/ProgressValue.h"
#include "WkCocos/Download/Comp/FileMD5.h"
#include "WkCocos/Download/Comp/CurlDL.h"

#include "WkCocos/Utils/ToolBox.h"

#include "WkCocos/Utils/Crypto.h"

#include "cocostudio/CocoStudio.h"

#include <curl/curl.h>
#include <curl/easy.h>

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

			DLCchecking::DLCchecking()
			try
			: _curl(nullptr)
			, _connectionTimeout(0)
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
								events->emit<Events::Error>(entity, "DLCchecking system");
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
								//is it possible to update ?
								bool dlc_update_allowed = false;

								//do we need to update ?
								bool dlc_update_required = false;

								std::string version = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "version", "");
								if (version == dllist->m_current_version) //if we have the exact same string : developer update or current version hotfix.
								{
									dlc_update_required = true;
								}
								else if (version.length() > 0 )  //we need to compare string to find if the online version is more recent
								{
									unsigned long lver = 0;
									try {
										lver = ToolBox::stoul(version);
										CCLOG("DLC at %s has Manifest version %lu", url.c_str(), lver);
									}
									catch (std::out_of_range oor)
									{
										lver = 0; //disabling update if online version is too high ( developer version )
									}

									unsigned long lcver = 0;
									try {
										lcver = ToolBox::stoul(dllist->m_current_version);
										CCLOG("Local DLC has Manifest version %lu", url.c_str(), lcver);
									}
									catch (std::out_of_range oor)
									{
										lcver = LONG_MAX; // if local version is too high, update should have been done before
									}

									if (lver > lcver) //not == to prevent LONG_MAX == LONG_MAX
									{
										dlc_update_required = true;
									}
								}

								std::string minAppVersion = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "minAppVersion", "error");
								if (minAppVersion != "error" && dllist->m_current_minAppVersion != "" )
								{
									//CAREFUL HERE with version comparison
									std::vector<unsigned long> mav = splitVersion(minAppVersion);
									std::vector<unsigned long> cmav = splitVersion(dllist->m_current_minAppVersion);

									dlc_update_allowed = true;
									while ( mav.size() < cmav.size() )
									{
										mav.push_back(0);
									}
									for (unsigned int i = 0; i < cmav.size(); ++i)
									{
										if (mav.at(i) > cmav.at(i))
										{
											dlc_update_allowed = false; break;
										}
									}
								}//if we cannot read the minimum app version. we dont download. better safe than sorry.

								if (dlc_update_allowed && dlc_update_required)
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
										newentity.assign<Comp::LocalFile>(filename);
										newentity.assign<Comp::RemoteMD5>(filehash);
										newentity.assign<Comp::RemoteFile>(dllist->m_url + "/" + dllist->m_verlist.back(), filename);
										
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

			void DLCchecking::setConnectionTimeout(unsigned long timeout)
			{
				_connectionTimeout = timeout;
			}

					
		}//namespace Systems
	}//namespace Download
}//namespace WkCocos

