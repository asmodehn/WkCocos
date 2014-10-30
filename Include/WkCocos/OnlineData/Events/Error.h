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
				Error(::App42::App42Response* r)
				: httpErrorCode(r->httpErrorCode)
				, app42ErrorCode(r->appErrorCode)
				, errorMessage(r->errorMessage)
				, errorDetails(r->errorDetails)
				{
				}

				int httpErrorCode;
				int app42ErrorCode;
				std::string errorMessage;
				std::string errorDetails;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_ERROR_H__
