#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__

#include "entityx/entityx.h"

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

#define FILE_DL_EXT ".dlding"

#include "curl/curl.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{

			/**
			* This system is a simple Entity System style Facade for Curl Easy Interface.
			* the only added behavior is the possibility to retry on some errors.
			* Create more instances of this system to allow for multiple simultaneous connection.
			*/
			struct CurlDL : public entityx::System<CurlDL>
			{
				static std::string curlError(int code);

				CurlDL();

				~CurlDL();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				//One Curl handle per system allows all downloads done by this system
				//to share the same connection.
				CURL* _curl;

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__
