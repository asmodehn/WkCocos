#ifndef __DFGAME_LOCALDATA_EVENTS_ERROR_H__
#define __DFGAME_LOCALDATA_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error> {
				Error(entityx::Entity e) : errored_entity(e) {}

				entityx::Entity errored_entity;
			};

		}//namespace Event
	}//namespace Loading
}//namespace dfgame


#endif // __DFGAME_LOCALDATA_EVENTS_ERROR_H__
