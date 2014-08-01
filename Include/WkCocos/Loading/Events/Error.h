#ifndef __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Events
		{
				struct Error : public entityx::Event<Error> {
				Error(entityx::Entity e) : errored_entity(e) {}

				entityx::Entity errored_entity;
			};

		}//namespace Event
	}//namespace LocalData
}//namespace dfgame


#endif // __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__
