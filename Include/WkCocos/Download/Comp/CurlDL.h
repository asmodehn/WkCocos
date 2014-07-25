#ifndef __DFGAME_DOWNLOAD_ENTITY_COMP_CURLDL_H__
#define __DFGAME_DOWNLOAD_ENTITY_COMP_CURLDL_H__

#include "cocos/cocos2d.h"

#include "entityx/entityx.h"

#include "curl/curl.h"

#include "WkCocos/Utils/ToolBox.h"

#include <string>
#include <future>

#define LOW_SPEED_LIMIT 30L
#define LOW_SPEED_TIME 5L

namespace WkCocos
{
	namespace Download
	{
		namespace Comp
		{
			//Component storing everything needed to Curl easy download
			struct CurlDL : entityx::Component<CurlDL> {
				CurlDL(std::string url,std::string path, CURLSH* share_handle = nullptr, short retries = 3)
				: m_retries(retries)
				, m_url(url)
				, m_path(path)
				, m_fp(nullptr)
				{
					CCLOG("Initializing curl easy handle for %s ...", m_url.c_str());
					m_easy_handle = curl_easy_init();

					if (share_handle)
					{
						curl_easy_setopt(m_easy_handle, CURLOPT_SHARE, share_handle);
					}
					curl_easy_setopt(m_easy_handle, CURLOPT_URL, m_url.c_str());
#ifdef _DEBUG
					curl_easy_setopt(m_easy_handle, CURLOPT_VERBOSE, 1L);
#endif
					curl_easy_setopt(m_easy_handle, CURLOPT_WRITEFUNCTION, download_file);
					curl_easy_setopt(m_easy_handle, CURLOPT_WRITEDATA, this);
					curl_easy_setopt(m_easy_handle, CURLOPT_ERRORBUFFER, error_buffer);
					curl_easy_setopt(m_easy_handle, CURLOPT_NOSIGNAL, 1L);
					curl_easy_setopt(m_easy_handle, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
					curl_easy_setopt(m_easy_handle, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);

					//we need to force async launch behavior here ( the default is deferred on android it seems )
					m_future = std::async(std::launch::async,[&]() -> CURLcode {
						CURLcode errcode;
						char* eff_url;
						curl_easy_getinfo(m_easy_handle, CURLINFO_EFFECTIVE_URL, &eff_url);
						do
						{
							CCLOG("Downloading %s ...", eff_url);
							errcode = curl_easy_perform(m_easy_handle);
							if (CURLE_OK != errcode)
							{
								m_retries--;
								CCLOG("Downloading cannot read from %s, error code is %s", eff_url, curlError(errcode).c_str());
							}
							//close the file pointer if needed
							if (m_fp)
							{
								fclose(m_fp), m_fp = nullptr;
							}
						} while (CURLE_OK != errcode && m_retries > 0);

						return errcode;
					});
				}

				~CurlDL()
				{
					curl_easy_cleanup(m_easy_handle);
				}

				std::string curlError(int code)
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

				static size_t download_file(void *ptr, size_t size, size_t nmemb, void *userdata)
				{
					CurlDL * instance = (CurlDL*)userdata;
					if (instance)
					{
						if (!instance->m_fp)
						{
							instance->m_fp = ToolBox::FOpen(instance->m_path.c_str(), "wb");
							if (!instance->m_fp)
							{
								CCLOG("can not create file %s", instance->m_path.c_str());
								return -1;
							}
						}
						//CCLOG("Writing %d bytes into %p", size*nmemb, instance->m_fp);
						size_t written = fwrite(ptr, size, nmemb, instance->m_fp);
						return written;
					}
					else
					{
						return -1;
					}

				}

				CURL* getEasyHandle()
				{
					return m_easy_handle;
				}

				std::future<CURLcode>& getFuture()
				{
					return m_future;
				}

				std::string getErrorMsg()
				{
					return std::string(error_buffer);
				}

			private:
				std::string m_path; //local path of the file
				std::string m_url; //url of the file
				CURL* m_easy_handle; //curl handle
				FILE* m_fp; //pointer to the temp file
				char error_buffer[CURL_ERROR_SIZE];
				std::future<CURLcode> m_future; //future to hold the result of the transfer
				unsigned short m_retries; //number of retries allowed to download this URL
			};

		}//namespace Comp
	}//namespace Download
}//namespace WkCocos

#endif // __DFGAME_DOWNLOAD_ENTITY_COMP_CURLDL_H__
