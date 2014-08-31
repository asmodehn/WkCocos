#ifndef __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__
#define __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__

#include "entityx/entityx.h"
//#include "WkCocos/OnlineData/Comp/OnlineData.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct ServerTime : public entityx::Event<ServerTime>
			{
				ServerTime(std::string serverTime) : serverTime(serverTime)
				{
					

				}

				std::string serverTime;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__
