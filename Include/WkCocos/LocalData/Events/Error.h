#ifndef __DFGAME_LOCALDATA_EVENTS_ERROR_H__
#define __DFGAME_LOCALDATA_EVENTS_ERROR_H__

#include "entityx/entityx.h"
#include "WkCocos/LocalData/Comp/LocalData.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error> {
				Error(entityx::Entity e) : errored_entity(e)
				{
					auto tempF = e.component<Comp::File>();
					if (tempF)
					{
						msg = tempF->getFilename() + " read/write error";
					}

					auto tempPD = e.component<Comp::PlayerData_v1>();
					if (tempPD)
					{
						msg = "Player data parse error";
					}
				}

				entityx::Entity errored_entity;

				std::string msg;

			};

		}//namespace Event
	}//namespace Loading
}//namespace dfgame


#endif // __DFGAME_LOCALDATA_EVENTS_ERROR_H__
