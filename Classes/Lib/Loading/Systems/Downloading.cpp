#include "WkCocos/Loading/Systems/Downloading.h"
#include "WkCocos/Loading/Events/Error.h"

#include "WkCocos/Loading/Comp/DataLoad.h"
#include "WkCocos/Loading/Comp/ProgressValue.h"

#include "WkCocos/Utils/Crypto.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include "WkCocos/Utils/ToolBox.h"

#include "cocos/cocos2d.h"
#include "cocostudio/CocoStudio.h"


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

			Downloading::Downloading()
			try
			: _curl(nullptr)
			, _connectionTimeout(0)
			, _retries(3)
			{

				CCLOG("Downloading Curl Init");
				_curl = curl_easy_init();
				if (!_curl)
				{
					throw std::logic_error("CANNOT INIT CURL");
				}
				CCLOG("Downloading Curl Init OK !");
			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start DLC downloading : %s", e.what());
			}

			Downloading::~Downloading()
			{
				curl_easy_cleanup(_curl);
			}

			void Downloading::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				for (auto entity : es->entities_with_components<Comp::DataDownload>())
				{
					entityx::ptr<Comp::DataDownload> dlfile = entity.component<Comp::DataDownload>();
					std::string url = dlfile->m_url;
					std::string filepath = dlfile->m_filepath;
					std::string fileurl = url + "/" + filepath;

					std::string current_md5 = computeMD5(filepath);

					auto error_fun = [events, &entity](entityx::ptr<Comp::DataDownload> dl)
					{
						dl->m_retries--;
						if (0 == dl->m_retries)
						{
							CCLOGERROR("ERROR Downloading %s from %s. CANCELLING.", dl->m_filepath.c_str(), dl->m_url.c_str());
							//signal error
							events->emit<Events::Error>(entity);
							//we give up on this entity
							entity.destroy();
						} 
					};

					//if we cannot read the file we will download it again
					if (current_md5 != dlfile->m_hash)
					{
						bool downloaded = false;
						// Create a file to save downloaded data.
						const std::string outFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + filepath + FILE_DL_EXT;
						CCLOG("Downloading  %s...", outFileName.c_str());
						FILE *fp = WkCocos::ToolBox::FOpen(outFileName, "wb");
						if (!fp)
						{
							//Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
							//	if (this->_delegate)
							//		this->_delegate->onError(ErrorCode::CREATE_FILE);
							//});
							CCLOG("can not create file %s", outFileName.c_str());

							//signal error
							events->emit<Events::Error>(entity);

							//TMP
							//we cannot do anything with this one : ignore it.
							entity.destroy();
						}
						else
						{
							// Download pacakge
							CURLcode res;
							curl_easy_setopt(_curl, CURLOPT_URL, fileurl.c_str());
#ifdef _DEBUG
							curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
							curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, download_file);
							curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
							curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
							curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, download_progress);
							curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
							curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
							curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
							curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);

							res = curl_easy_perform(_curl);
							//TODO : change to multi_perform
							//http://curl.haxx.se/libcurl/c/curl_easy_perform.html
							if (res != 0)
							{

								CCLOG("Downloading can not read from %s, error code is %d", url.c_str(), res);
								error_fun(dlfile);

							}
							else
							{
								downloaded = true;
							}
							fclose(fp);
						}

						if (downloaded)
						{
							std::string current_md5 = computeMD5(outFileName.c_str());
							if (current_md5 != dlfile->m_hash)
							{
								CCLOG("error downloading %s", fileurl.c_str());
								CCLOG("error expected MD5 %s", dlfile->m_hash.c_str());
								CCLOG("error actual MD5 %s", current_md5.c_str());
								error_fun(dlfile);
							}
							else
							{
								const std::string finalFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + filepath;
								//download is fine.
								if (cocos2d::FileUtils::getInstance()->isFileExist(finalFileName))
								{
									remove(finalFileName.c_str());
								}
								if (0 != rename(outFileName.c_str(), finalFileName.c_str()))
								{
									CCLOG("error renaming %s to %s", outFileName.c_str(), fileurl.c_str());
								}
								CCLOG("succeed downloading package %s", fileurl.c_str());

								//we dont need to register the new files in manifest,
								//because we compute locally the md5 before deciding to download or not
								//=> If the file is at the expeted place with proper signature, download will not happen again.



								entity.destroy();
							}
						}

						//exit this loop. one per update is enough
						break;
					}
					else
					{
						//md5 signature was hte same : we can ignore this download

						entity.destroy();

						//keep looping
					}
				}
			};


			std::string Downloading::computeMD5(std::string filepath)
			{
				std::string current_md5 = "";

				//CCLOG(" CHECKING MD5 for %s", filepath.c_str());
				cocos2d::Data filedata = cocos2d::FileUtils::getInstance()->getDataFromFile(filepath.c_str());
				
				if (!filedata.isNull()) //read successful
				{
					unsigned char hashbuffer[Crypto::MD5_BUFFER_LENGTH];
					Crypto::MD5((void*)filedata.getBytes(), filedata.getSize(), hashbuffer);
					//converting to string with hex
					std::stringstream ss;
					ss << std::hex << std::setfill('0');
					for (int i = 0; i < sizeof(hashbuffer); ++i)
					{
						ss << std::setw(2) << static_cast<unsigned int>(hashbuffer[i]);
					}
					current_md5 = ss.str();
					//std::cout << filepath << " : " << current_md5 << std::endl;
				}
				return current_md5;
			}

			void Downloading::setConnectionTimeout(unsigned int timeout)
			{
				_connectionTimeout = timeout;
			}
					
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

