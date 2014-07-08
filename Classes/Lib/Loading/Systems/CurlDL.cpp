#include "WkCocos/Loading/Systems/CurlDL.h"
#include "WkCocos/Loading/Events/Error.h"

#include "WkCocos/Loading/Comp/CurlDL.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include "WkCocos/Utils/ToolBox.h"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{
			//a simple container to pass entities to C-style callbacks
			struct EntityContainer
			{
				EntityContainer(entityx::Entity e, entityx::ptr<entityx::EventManager> ev) : entity(e),events(ev) {}
				entityx::Entity entity;
				entityx::ptr<entityx::EventManager> events;
			};

			static size_t write_data(void *ptr, size_t size, size_t nmemb, void *userdata)
			{
				std::string * dirlist = (std::string*)userdata;
				dirlist->append((char*)ptr, size * nmemb);

				return (size * nmemb);
			}

			static size_t download_file(void *ptr, size_t size, size_t nmemb, void *userdata)
			{
				EntityContainer * econt = (EntityContainer*)userdata;
				entityx::Entity entity = econt->entity;
				
				if (!entity.valid())
				{
					return -1;
				}

				auto path = entity.component<Comp::TempFile>();
				auto pointer = entity.component<Comp::TempFileP>();

				FILE* fp;
				if (path)
				{
					if (!pointer)
					{
						fp = fopen(path->getPath().c_str(), "wb");
						if (!fp)
						{
							CCLOG("can not create file %s", path->getPath().c_str());

							//signal error
							econt->events->emit<Events::Error>(entity);

							//TMP
							//we cannot do anything more with this one : ignore it.
							entity.destroy();

							return -1;
						}
						else
						{
							entity.assign<Comp::TempFileP>(fp);
						}
					}
					else
					{
						fp = pointer->getFileP();
					}
					size_t written = fwrite(ptr, size, nmemb, fp);
					return written;
				}
				else
				{
					return -1;
				}
				
			}

			std::string CurlDL::curlError(int code)
			{
				std::string s;
				switch (code) {
				case     CURLM_BAD_HANDLE:         s = "CURLM_BAD_HANDLE";         break;
				case     CURLM_BAD_EASY_HANDLE:    s = "CURLM_BAD_EASY_HANDLE";    break;
				case     CURLM_OUT_OF_MEMORY:      s = "CURLM_OUT_OF_MEMORY";      break;
				case     CURLM_INTERNAL_ERROR:     s = "CURLM_INTERNAL_ERROR";     break;
				case     CURLM_UNKNOWN_OPTION:     s = "CURLM_UNKNOWN_OPTION";     break;
				case     CURLM_LAST:               s = "CURLM_LAST";               break;
				case     CURLM_BAD_SOCKET:         s = "CURLM_BAD_SOCKET";         break;
				default: s = "CURLM_unknown";
					break;
				}
				return s;
			}

			CurlDL::CurlDL()
			try
			: _curl(nullptr)
			{
				CCLOG("CurlDL Init");
				_curl = curl_easy_init();
				if (!_curl)
				{
					throw std::logic_error("CANNOT INIT CURL");
				}
				CCLOG("CurlDL Init OK !");
			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start downloading : %s", e.what());
			}

			CurlDL::~CurlDL()
			{
				curl_easy_cleanup(_curl);
			}

			void CurlDL::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::LocalFile> localfile;
				entityx::ptr<Comp::LocalMD5> localsig;
				entityx::ptr<Comp::RemoteFile> remotefile;
				entityx::ptr<Comp::RemoteMD5> remotesig;
				for (auto entity : es->entities_with_components(localfile,localsig,remotefile,remotesig))
				{
					if (localsig->getMD5() == remotesig->getMD5())
					{
						//This component is not needed anymore.
						entity.remove<Comp::RemoteFile>();

						//we can actually register progress already, as this entity doesnt need to be downloaded :
						entity.destroy();
					}
					else if (!entity.component<Comp::CurlDL>() && !entity.component<Comp::TempFile>()) //if we havent finished downloading it yet.
					{
						// Create a file to save downloaded data.
						const std::string outFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + remotefile->getPath() + FILE_DL_EXT;
						CCLOG("Downloading  %s...", remotefile->getPath().c_str());

						//register the temp file in entity.
						//md5 will be checked on it before deciding to validate download or not.
						entity.assign<Comp::TempFile>(outFileName);

						if (!entity.component<Comp::CurlDL>())
						{

							//build full URL
							std::string fullURL = remotefile->getURL() + "/" + remotefile->getPath();

							// Download starts
							auto dlfile = entity.assign<Comp::CurlDL>();

							//to pass the entity to the callback
							EntityContainer entCont(entity,events);

							CURLcode res;
							curl_easy_setopt(_curl, CURLOPT_URL, fullURL.c_str());
#ifdef _DEBUG
							curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
							curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, download_file);
							curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &entCont);
							//curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, dlfile->getErrorBuffer());
							curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
							curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
							curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);

							res = curl_easy_perform(_curl);
							//this is blocking...

							//http://curl.haxx.se/libcurl/c/curl_easy_perform.html
							if (CURLE_OK != res)
							{
								unsigned short retries = dlfile->consumeRetry();
								CCLOG("Downloading can not read from %s, error code is %s", fullURL, curlError(res));

								//removing components to allow retry on next update
								entity.remove<Comp::TempFile>();
								entity.remove<Comp::CurlDL>();

								if (0 >= retries)
								{
									CCLOGERROR("ERROR Downloading %s from %s. CANCELLING.", remotefile->getPath().c_str(), remotefile->getURL().c_str());
									//signal error
									events->emit<Events::Error>(entity);
									//we give up on this entity
									entity.destroy();
								}
							}
							else
							{
								//download successfully finished.
								entity.remove<Comp::CurlMultiDL>();

								auto tfp = entity.component<Comp::TempFileP>();
								if (tfp && tfp->getFileP())
								{
									fclose(tfp->getFileP()); //close the temp file
								}

							}
						}
						else
						{
							//a download was started and not finished properly.
							//we shouldnt be here with synchronous curl.
						}

						//exit this loop. one per update is enough
						break;
					}
					else
					{ //this is an entity that was downloaded but not checked yet.

						//nothing to do here
					}

				}
			}

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

