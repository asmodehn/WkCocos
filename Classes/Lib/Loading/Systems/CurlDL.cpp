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
			, _retries(3)
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
					else if (!entity.component<Comp::TempFile>()) //if we havent finished downloaded it yet.
					{
						// Create a file to save downloaded data.
						const std::string outFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + remotefile->getPath() + FILE_DL_EXT;
						CCLOG("Downloading  %s...", remotefile->getPath().c_str());

						FILE *fp = WkCocos::ToolBox::FOpen(outFileName, "wb");
						if (!fp)
						{
							CCLOG("can not create file %s", outFileName);

							//signal error
							events->emit<Events::Error>(entity);

							//TMP
							//we cannot do anything with this one : ignore it.
							entity.destroy();
						}
						else
						{
							if (!entity.component<Comp::CurlDL>())
							{

								//build full URL
								std::string fullURL = remotefile->getURL() + "/" + remotefile->getPath();

								// Download starts
								auto dlfile = entity.assign<Comp::CurlDL>();

								CURLcode res;
								curl_easy_setopt(_curl, CURLOPT_URL, fullURL.c_str());
#ifdef _DEBUG
								curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
								curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, download_file);
								curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
								//curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, dlfile->getErrorBuffer());
								curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
								curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, download_progress);
								curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
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
									//register the temp file in entity.
									//md5 will be checked on it before deciding to validate download or not.
									entity.assign<Comp::TempFile>(outFileName);
								}
							}
							else
							{
								//a download was started and not finished properly.
								//we shouldnt be here with synchronous curl.
							}
						}
						fclose(fp);

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

