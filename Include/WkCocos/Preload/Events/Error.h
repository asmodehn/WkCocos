#ifndef __DFGAME_PRELOAD_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_PRELOAD_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error>
			{
				Error(entityx::Entity e) : errored_entity(e) {}

				entityx::Entity errored_entity;
			};

		}//namespace Event
	}//namespace Preload
}//namespace WkCocos


#endif // __DFGAME_PRELOAD_ENTITY_EVENTS_ERROR_H__
