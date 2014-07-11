#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__

#include "entityx/entityx.h"

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
				CurlDL(unsigned short concurrent_dl = 1);

				~CurlDL();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

			protected:

				//curl shared settings
				CURLSH* _curl_share;

				unsigned short m_concurrent_dl;
				unsigned short m_concurrent_dl_max;
			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_CURLDL_H__
