#ifndef __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__
#define __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__

#include "entityx/entityx.h"
#include "WkCocos/Utils/ToolBox.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct ServerTime : public entityx::Event<ServerTime>
			{
				ServerTime(std::string s_iso8601)// : serverTime(s_iso8601)
				{
					serverTime = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					char* pos;
					serverTime.tm_year = strtoul(s_iso8601.c_str(), &pos, 10);
					serverTime.tm_mon = strtoul(++pos, &pos, 10);
					serverTime.tm_mday = strtoul(++pos, &pos, 10);
					serverTime.tm_hour = strtoul(++pos, &pos, 10);
					serverTime.tm_min = strtoul(++pos, &pos, 10);
					serverTime.tm_sec = strtoul(++pos, &pos, 10);
				}

				struct tm serverTime;
				//std::string serverTime;
			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_SERVERTIME_H__
