#ifndef __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
#define __DFGAME_ONLINEDATA_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error> {
				Error(entityx::Entity e) : errored_entity(e) {}

				entityx::Entity errored_entity;
			};

		}//namespace Events
	}//namespace OnlineData
}//namespace dfgame


#endif // __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
