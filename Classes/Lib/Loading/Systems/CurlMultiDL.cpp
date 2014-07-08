#include "WkCocos/Loading/Systems/CurlMultiDL.h"
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

			std::shared_ptr<struct event_base*> CurlMultiDL::s_evbase;
			std::shared_ptr<struct event*> CurlMultiDL::s_timer_event;
			std::shared_ptr<CURLM*> CurlMultiDL::s_multicurl;

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
			
			/* Clean up the SockInfo structure */
			void CurlMultiDL::remsock(SockInfo *f)
			{
				if (f) {
					if (f->evset)
						event_free(f->ev);
					free(f);
				}
			}

			/* Assign information to a SockInfo structure */
			void CurlMultiDL::setsock(SockInfo*f, curl_socket_t s, CURL*e, int act, CurlMultiDL*g)
			{
				int kind =
					(act&CURL_POLL_IN ? EV_READ : 0) | (act&CURL_POLL_OUT ? EV_WRITE : 0) | EV_PERSIST;

				f->sockfd = s;
				f->action = act;
				f->easy = e;
				if (f->evset)
					event_free(f->ev);
				f->ev = event_new(*(g->evbase), f->sockfd, kind, event_cb, g);
				f->evset = 1;
				event_add(f->ev, NULL);
			}

			/* Initialize a new SockInfo structure */
			void CurlMultiDL::addsock(curl_socket_t s, CURL *easy, int action, CurlMultiDL *g)
			{
				SockInfo *fdp = (SockInfo *)calloc(sizeof(SockInfo), 1);

				fdp->global = g;
				setsock(fdp, s, easy, action, g);
				curl_multi_assign(*(g->multicurl), s, fdp);
			}

			/* CURLMOPT_SOCKETFUNCTION */
			int CurlMultiDL::sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
			{
				CurlMultiDL *g = (CurlMultiDL*)cbp;
				SockInfo *fdp = (SockInfo*)sockp;
				const char *whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE" };

				fprintf(stdout, "socket callback: s=%d e=%p what=%s ", s, e, whatstr[what]);
				if (what == CURL_POLL_REMOVE) {
					fprintf(stdout, "\n");
					remsock(fdp);
				}
				else {
					if (!fdp) {
						fprintf(stdout, "Adding data: %s\n", whatstr[what]);
						addsock(s, e, what, g);
					}
					else {
						fprintf(stdout,
							"Changing action from %s to %s\n",
							whatstr[fdp->action], whatstr[what]);
						setsock(fdp, s, e, what, g);
					}
				}
				return 0;
			}

			/* Update the event timer after curl_multi library calls */
			int CurlMultiDL::multi_timer_cb(CURLM *multi, long timeout_ms, CurlMultiDL *cmdl)
			{
				struct timeval timeout;
				(void)multi; /* unused */

				timeout.tv_sec = timeout_ms / 1000;
				timeout.tv_usec = (timeout_ms % 1000) * 1000;
				fprintf(stdout, "multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);
				int timer_added = evtimer_add(*(cmdl->timer_event), &timeout);
				return timer_added;
			}

			/* Called by libevent when we get action on a multi socket */
			void CurlMultiDL::event_cb(int fd, short kind, void *userp)
			{
				CurlMultiDL *g = (CurlMultiDL*)userp;
				CURLMcode rc;

				int action =
					(kind & EV_READ ? CURL_CSELECT_IN : 0) |
					(kind & EV_WRITE ? CURL_CSELECT_OUT : 0);

				rc = curl_multi_socket_action(*(g->multicurl), fd, action, &g->still_running);
				mcode_or_die("event_cb: curl_multi_socket_action", rc);

				check_multi_info(g);
				if (g->still_running <= 0) {
					fprintf(stdout, "last transfer done, kill timeout\n");
					if (evtimer_pending(*(g->timer_event), NULL)) {
						evtimer_del(*(g->timer_event));
					}
				}
			}

			/* Called by libevent when our timeout expires */
			void CurlMultiDL::timer_cb(int fd, short kind, void *userp)
			{
				CurlMultiDL *g = (CurlMultiDL *)userp;
				CURLMcode rc;
				(void)fd; /* unused */
				(void)kind; /* unused */

				rc = curl_multi_socket_action(*(g->multicurl), CURL_SOCKET_TIMEOUT, 0, &g->still_running);
				mcode_or_die("timer_cb: curl_multi_socket_action", rc);
				check_multi_info(g);
			}

			/* Check for completed transfers, and remove their easy handles */
			void CurlMultiDL::check_multi_info(CurlMultiDL *g)
			{
				char *eff_url;
				CURLMsg *msg;
				int msgs_left;
				ConnInfo *conn;
				CURL *easy;
				CURLcode res;

				fprintf(stdout, "REMAINING: %d\n", g->still_running);
				while ((msg = curl_multi_info_read(*(g->multicurl), &msgs_left))) {
					if (msg->msg == CURLMSG_DONE) {
						easy = msg->easy_handle;
						res = msg->data.result;
						curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
						curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
						fprintf(stdout, "DONE: %s => (%d) %s\n", eff_url, res, conn->error);
						curl_multi_remove_handle(*(g->multicurl), easy);
						free(conn->url);
						curl_easy_cleanup(easy);
						free(conn);
					}
				}
			}

			std::string CurlMultiDL::curlError(int code)
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

			/* Die if we get a bad CURLMcode somewhere */
			void CurlMultiDL::mcode_or_die(const char *where, CURLMcode code)
			{
				if (CURLM_OK != code) {
					fprintf(stdout, "ERROR: %s returns %s\n", where, curlError(code));
					exit(code);
				}
			}

			CurlMultiDL::CurlMultiDL()
			try
			: evbase() //empty shared_ptr
			, multicurl() //empty shared_ptr
			, timer_event() //empty shared_ptr
			, _curl(nullptr)
			{
				//static member initialization if needed ( we are the first instance )
				if (!s_evbase)
				{
					s_evbase = std::make_shared<struct event_base*>(event_base_new());
				}
				evbase = s_evbase ;

				if (!s_multicurl)
				{
					s_multicurl = std::make_shared<CURLM*>(curl_multi_init());
					if (!s_multicurl)
					{
						throw std::logic_error("CANNOT INIT MULTICURL");
					}

					/* setup the generic multi interface options we want */
					curl_multi_setopt(*(s_multicurl), CURLMOPT_SOCKETFUNCTION, sock_cb);
					curl_multi_setopt(*(s_multicurl), CURLMOPT_SOCKETDATA, this);
					curl_multi_setopt(*(s_multicurl), CURLMOPT_TIMERFUNCTION, multi_timer_cb);
					curl_multi_setopt(*(s_multicurl), CURLMOPT_TIMERDATA, this);
				}
				multicurl = s_multicurl;

				if (!s_timer_event)
				{
					s_timer_event = std::make_shared<event*>(evtimer_new(*(evbase), timer_cb, this));
				}
				timer_event = s_timer_event;

				//instance member initialization
				_curl = curl_easy_init();
				if (!_curl)
				{
					throw std::logic_error("CANNOT INIT EASYCURL");
				}

				// we don't call any curl_multi_socket*() function yet as we have no handles added!
				
				CCLOG("CurlMultiDL Init OK !");
			}
			catch (std::exception e)
			{
				CCLOGERROR("Cannot start multithreaded download : %s", e.what());
			}

			CurlMultiDL::~CurlMultiDL()
			{
				curl_easy_cleanup(_curl);

				//static member cleanup if needed
				timer_event.reset(); //decreasing the smart pointer count
				if (s_timer_event.unique())
				{
					event_free(*(s_timer_event));
				}
				multicurl.reset();
				if (s_multicurl.unique())
				{
					curl_multi_cleanup(*(s_multicurl));
				}
				evbase.reset();
				if (s_evbase.unique())
				{
					event_base_free(*(s_evbase));
				}
			}

			void CurlMultiDL::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
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
					else if (!entity.component<Comp::CurlMultiDL>() && !entity.component<Comp::TempFile>()) 
					{//if we havent started downloaded yet and we arent waiting for confirmation.

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
							if (!entity.component<Comp::CurlMultiDL>())
							{

								//build full URL
								std::string fullURL = remotefile->getURL() + "/" + remotefile->getPath();

								// Download starts
								auto dlfile = entity.assign<Comp::CurlMultiDL>();

								dlfile->setFileP(fp);

								CURLMcode res;
								curl_easy_setopt(_curl, CURLOPT_URL, fullURL.c_str());
#ifdef _DEBUG
								curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
#endif
								curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, download_file);
								curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
								curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, dlfile->getErrorBuffer());
								curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
								curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
								curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
								fprintf(stdout, "Adding easy %p to multi %p (%s)\n", _curl,*multicurl, fullURL.c_str());
								res = curl_multi_add_handle(*multicurl, _curl);
								// Note that the add_handle() will set a time-out to trigger very soon so
								// that the necessary socket_action() call will be called by this app

								/*
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
								*/
							}
							else
							{
								//a download was started and not finished properly.
							}
						}

						//exit this loop. one per update is enough
						break;
					}
					else
					{ //this is an entity that was downloaded but not checked yet.

						//nothing to do here
					}

				}

				//triggering all callbacks on events
				event_base_dispatch(*(evbase));

			}

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

