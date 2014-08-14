#ifndef __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
#define __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error>
			{
				Error(entityx::Entity e, std::string msg) : errored_entity(e), msg(msg) {}

				entityx::Entity errored_entity;

				std::string msg;

			};

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_ENTITY_EVENTS_ERROR_H__
