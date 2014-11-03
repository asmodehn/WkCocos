#ifndef __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
#define __DFGAME_ONLINEDATA_EVENTS_ERROR_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct Error : public entityx::Event<Error>
			{
				Error(entityx::Entity::Id a_id, ::App42::App42Response* r)
					: id(a_id)
					, httpErrorCode(r->httpErrorCode)
					, app42ErrorCode(r->appErrorCode)
					, errorMessage(r->errorMessage)
					, errorDetails(r->errorDetails)
				{
				}

				Error(entityx::Entity::Id a_id, std::string r)
					: id(a_id)
					, httpErrorCode(0)
					, app42ErrorCode(0)
					, errorMessage("timeout")
					, errorDetails(r)
				{
				}

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
