#include "WkCocos/Download/Systems/CurlDL.h"
#include "WkCocos/Download/Events/Error.h"

#include "WkCocos/Download/Comp/FileMD5.h"
#include "WkCocos/Download/Comp/CurlDL.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include "WkCocos/Utils/ToolBox.h"

#include <future>

namespace WkCocos
{
	namespace Download
	{
		namespace Systems
		{

			CurlDL::CurlDL(unsigned short concurrent_dl)
			try
			: _curl_share(nullptr)
			, m_concurrent_dl_max(concurrent_dl)
			, m_concurrent_dl(0)
			{
				CCLOG("CurlDL Init");
				_curl_share = curl_share_init();
				if (!_curl_share)
				{
					throw std::logic_error("CANNOT INIT CURL SHARE");
				}

				curl_share_setopt(_curl_share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE | CURL_LOCK_DATA_DNS | CURL_LOCK_DATA_SSL_SESSION | CURL_LOCK_DATA_CONNECT);
				CCLOG("CurlDL Init OK !");

			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start downloading : %s", e.what());
			}

			CurlDL::~CurlDL()
			{
				//cleaning up all easy handles
				curl_share_cleanup(_curl_share);
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
					else if (m_concurrent_dl < m_concurrent_dl_max && !entity.component<Comp::CurlDL>() && !entity.component<Comp::TempFile>()) //if we havent finished downloading it yet.
					{
						// Create a file to save downloaded data.
						const std::string outFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + remotefile->getPath() + FILE_DL_EXT;
						CCLOG("Downloading  %s...", remotefile->getPath().c_str());

						//register the temp file in entity.
						//md5 will be checked on it before deciding to validate download or not.
						entity.assign<Comp::TempFile>(outFileName);

						//build full URL
						std::string fullURL = remotefile->getURL() + "/" + remotefile->getPath();

						// Download starts
						++m_concurrent_dl;
						auto dlfile = entity.assign<Comp::CurlDL>(fullURL, outFileName, _curl_share);
							
					}
					else if (entity.component<Comp::CurlDL>()) //this is an entity with download ongoing
					{
						auto dl = entity.component <Comp::CurlDL>();
						if (dl->getFuture().wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
						{
							//download is done
							--m_concurrent_dl;
							entity.remove<Comp::CurlDL>();

							CURLcode errcode = dl->getFuture().get();
							if (CURLE_OK != errcode)
							{	
								CCLOGERROR("CURL ERROR : %s", dl->getErrorMsg().c_str());
								CCLOGERROR("ERROR Downloading %s from %s. CANCELLING.", remotefile->getPath().c_str(), remotefile->getURL().c_str());
								//signal error
								events->emit<Events::Error>(entity, "CurlDL system");
								//we give up on this entity
								entity.destroy();
							}
						}
					}
					else if (entity.component<Comp::TempFile>())
					{ //this is an entity that was downloaded but not checked yet.

						//nothing to do here
					}
					else
					{ //we have reached our max download limit
						break; //break out of the loop. no need to check more
					}

				}
			}

		}//namespace Systems
	}//namespace Download
}//namespace WkCocos

