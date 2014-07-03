#ifndef __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace dfgame
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
	}//namespace Loading
}//namespace dfgame


#endif // __DFGAME_LOADING_ENTITY_EVENTS_ERROR_H__
