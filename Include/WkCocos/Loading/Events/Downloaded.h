#ifndef __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__
#define __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Events
		{
			struct Downloaded : public entityx::Event<Downloaded>
			{
				Downloaded(entityx::Entity e, entityx::ptr<entityx::EventManager> events)
				: downloaded_entity(e)
				, downloaded_events(events)
				{}

				entityx::Entity downloaded_entity;
				entityx::ptr<entityx::EventManager> downloaded_events;
			};

		}//namespace Event
	}//namespace Loading
}//namespace WkCocos


#endif // __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__
