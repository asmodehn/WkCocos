#ifndef __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__
#define __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__

#include "entityx/entityx.h"
//#include "WkCocos/OnlineData/Comp/OnlineData.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct PlayersList : public entityx::Event<PlayersList>
			{
				PlayersList(std::string playerslist) : eventMessage(playerslist) { }

				std::string eventMessage;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_PLAYERSLIST_H__
