#ifndef __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__
#define __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct PlayersList : public entityx::Event < PlayersList >
			{
				PlayersList(std::map<std::string, std::string> playersList, int recordCount) : eventMessage(playersList), recordCount(recordCount) { }
				std::map<std::string, std::string> eventMessage;
				int recordCount;
			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__
