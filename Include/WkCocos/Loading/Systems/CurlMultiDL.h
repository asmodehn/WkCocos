#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__

#include "entityx/entityx.h"

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

#define FILE_DL_EXT ".dlding"

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

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_CURLMULTIDL_H__
