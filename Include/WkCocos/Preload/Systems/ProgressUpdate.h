#ifndef __DFGAME_PRELOAD_ENTITY_SYSTEMS_PROGRESSUPDATE_H__
#define __DFGAME_PRELOAD_ENTITY_SYSTEMS_PROGRESSUPDATE_H__

#include "cocos2d.h"
#include "entityx/entityx.h"

#include "WkCocos/Preload/Comp/ProgressValue.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{
			//system readong data to determine which loading component must be added.
			struct ProgressUpdate : public entityx::System<ProgressUpdate> {

				ProgressUpdate(std::function<void(float)> a_setProgressPercent);

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				std::function<void(float)> setProgressPercent;

				unsigned int totalProgValMax;
				unsigned int curProgVal;

			};
		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_SYSTEMS_PROGRESSUPDATE_H__
