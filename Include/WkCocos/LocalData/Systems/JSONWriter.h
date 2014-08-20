#ifndef __WKCOCOS_LOCALDATA_SYSTEMS_JSONWRITER_H__
#define __WKCOCOS_LOCALDATA_SYSTEMS_JSONWRITER_H__

//#include "cocos2d.h"

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Systems
		{
			//system reading data to determine which loading component must be added.
			struct JSONWriter : public entityx::System<JSONWriter>
			{
				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

			};

		}//namespace Systems
	}//namespace LocalData
}//namespace WkCocos

#endif // __WKCOCOS_LOCALDATA_SYSTEMS_JSONWRITER_H__
