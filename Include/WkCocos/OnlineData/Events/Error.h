#ifndef __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
#define __DFGAME_ONLINEDATA_EVENTS_ERROR_H__

#include "entityx/entityx.h"

#include "WkCocos/OnlineData/RequestStatus.h"

#include "Common/App42API.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error>
			{
				Error(entityx::Entity::Id a_id, ::App42::App42Response* r);;

				Error(RequestStatus rs);

				Error(entityx::Entity::Id a_id, std::string r);

				entityx::Entity::Id id;
				int httpErrorCode;
				int app42ErrorCode;
				std::string errorMessage;
				std::string errorDetails;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
