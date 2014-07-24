#ifndef __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__
#define __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
			struct Downloaded : public entityx::Event<Downloaded>
			{
				Downloaded(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events)
				: downloaded_entities(es)
				, downloaded_events(events)
				{}

				entityx::ptr<entityx::EntityManager> downloaded_entities;
				entityx::ptr<entityx::EventManager> downloaded_events;
			};

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__
