#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__

#include "entityx/entityx.h"

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

#define FILE_DL_EXT ".dlding"

#include "curl/curl.h"
#include "event2/event.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{

			//system managing downloading. N at a time.
			struct CurlMultiDL : public entityx::System<CurlMultiDL>
			{
				CurlMultiDL();

				~CurlMultiDL();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				static std::string curlError(int code);

				//CURL callbacks

				// TODO : MOVE SOMEWHERE ELSE ??
				/* Information associated with a specific socket */
				typedef struct _SockInfo
				{
					curl_socket_t sockfd;
					CURL *easy;
					int action;
					long timeout;
					struct event *ev;
					int evset;
					CurlMultiDL *global;
				} SockInfo;
				

				static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp);

				static void addsock(curl_socket_t s, CURL *easy, int action, CurlMultiDL *g);

				static void setsock(SockInfo*f, curl_socket_t s, CURL*e, int act, CurlMultiDL*g);

				static void remsock(SockInfo *f);


				static int multi_timer_cb(CURLM *multi, long timeout_ms, CurlMultiDL *lm);

				static void mcode_or_die(const char *where, CURLMcode code);

				static void event_cb(int fd, short kind, void *userp);

				static void timer_cb(int fd, short kind, void *userp);

				static void check_multi_info(CurlMultiDL *g);

			protected:
				//One Curl handle per system allows all downloads done by this system
				//to share the same connection.
				CURL* _curl;

				int still_running;

				//original multi setup
				static std::shared_ptr<struct event_base*> s_evbase;
				static std::shared_ptr<struct event*> s_timer_event;
				static std::shared_ptr<CURLM*> s_multicurl;

				//local copies to keep track of instances created and be able to cleanup when needed

				std::shared_ptr<struct event_base*> evbase;
				std::shared_ptr<struct event*> timer_event;
				std::shared_ptr<CURLM*> multicurl;

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__
