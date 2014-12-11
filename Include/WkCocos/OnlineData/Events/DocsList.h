#ifndef __DFGAME_ONLINEDATA_EVENTS_DOCSLIST_H__
#define __DFGAME_ONLINEDATA_EVENTS_DOCSLIST_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct DocsList : public entityx::Event < DocsList >
			{
				DocsList(std::vector<std::map<std::string, std::string>> docsList) : eventMessage(docsList) { }
				std::vector<std::map<std::string, std::string>> eventMessage;
			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_DOCSLIST_H__
