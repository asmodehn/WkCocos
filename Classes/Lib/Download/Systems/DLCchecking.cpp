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
				if (vstr.at(0) == 'v')
				{
					//removing first 'v'
					vstr = vstr.substr(1);
				}

				std::vector<unsigned long> sv;
				std::string delimiters = ".";
				size_t current;
				size_t next = -1;
				do
				{
					current = next + 1;
					next = vstr.find_first_of(delimiters, current);
					//std::cout << vstr.substr(current, next - current) << std::endl;
					unsigned long tmp = 0;
					try
					{
						tmp = ToolBox::stoul(vstr.substr(current, next - current));
					}
					catch (std::out_of_range oor)
					{
						tmp = ULONG_MAX;
					}
					sv.push_back(tmp);
				} while (next != std::string::npos);
				return sv;
			}

			std::string DLCchecking::joinVersion(std::vector<unsigned long> version)
			{
				std::string vstr = "v";
				for (unsigned long num : version)
				{
					vstr += std::to_string(num) + ".";
				}
				vstr.pop_back(); // removing trialing '.'
				return vstr;
			}

			bool DLCchecking::versionIsLess(const std::string & v1, const std::string & v2)
			{
				std::vector<unsigned long> v1l = splitVersion(v1);
				std::vector<unsigned long> v2l = splitVersion(v2);
				bool less = true;
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
					if (v1l.at(i) > v2l.at(i))
					{
						less = false; break;
					}
				}
#ifdef _DEBUG
				if (less)
				{
					std::cout << "Version " << v1 << " <= " << v2 << std::endl;
				}
				else
				{
					std::cout << "Version " << v1 << " > " << v2 << std::endl;
				}
#endif
				return less;
			}


			void DLCchecking::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				for (auto entity : es->entities_with_components<Comp::DataVerCheck>())
				{
					entityx::ptr<Comp::DataVerCheck> dllist = entity.component<Comp::DataVerCheck>();
					std::string url = dllist->m_url;

					if (!dllist->m_verlist.empty())
					{
						//order the list of versions ascendently
						std::sort(dllist->m_verlist.begin(), dllist->m_verlist.end(), std::bind(&DLCchecking::versionIsLess,this,std::placeholders::_1,std::placeholders::_2) );
						//needed to make sure we just need to check from the last one.

						//we need to check if the version url is valid ( contains a manifest.json )
						url += "/" + dllist->m_verlist.back() + "/manifest.json";
						m_manifest = "";
						CCLOG("DLCchecking reading from %s", url.c_str());

						//list all files at given url
						CURLcode res;
						curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
#ifdef _DEBUG
						//curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
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
							//read the downloaded manifest to compare versions
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
								if ( version != "") //if we cannot read the minimum app version. we dont download. better safe than sorry.
								{
									if (version == dllist->m_current_version) //if we have the exact same string : developer update or current version hotfix.
									{
										dlc_update_required = true;
										dlc_update_allowed = true;
									}
									else //we need to compare strings to find if the online version is more recent, and if it is supported on current code version
									{
										//extracting minimum code version required
										std::vector<unsigned long> varr = splitVersion(version);
										unsigned long version4 = varr.back();
										varr.pop_back();
										std::string version3 = joinVersion(varr);

										//NOTE : we might want to use this instead of extracting from dlcconfig.json : 
										//std::string cur_ver = Utils::jni::Utils::getVersionName();

										std::vector<unsigned long> cvarr = splitVersion(dllist->m_current_version);
										unsigned long cur_version4 = cvarr.back();
										cvarr.pop_back();
										std::string cur_version3 = joinVersion(cvarr);
										
										//check if online version can run on local app version
										if (versionIsLess(version3, cur_version3))
										{
											dlc_update_allowed = true;

											//check if update is still required
											if ( cur_version4 < version4 && version4 != ULONG_MAX ) //dev verison must never update normal version
											{
												dlc_update_required = true;
											}

										}
									}
								} // ( version != "")

								if (dlc_update_required && dlc_update_allowed)
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
									//updating from one data url should be enough.
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

