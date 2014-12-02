#include "WkCocos/OnlineData/Events/Error.h"


namespace WkCocos
{
	namespace OnlineData
	{
        namespace Events
        {
            Error::Error(entityx::Entity::Id a_id, ::App42::App42Response* r)
                : id(a_id)
                , httpErrorCode(r->httpErrorCode)
                , app42ErrorCode(r->appErrorCode)
                , errorMessage(r->errorMessage)
                , errorDetails(r->errorDetails)
            {
            }

            Error::Error(RequestStatus rs)
                : id(rs.getRequestId())
                , httpErrorCode(-1) //TODO : think about correct default error values ( same as app42 ? )...
                , app42ErrorCode(-1)
                , errorMessage("")
                , errorDetails("")
            {
                auto httperr = rs.getErrors().find(Impl::HTTP);
                if (httperr != rs.getErrors().end() )
                {
                    httpErrorCode = httperr->second.code;
                    errorMessage = httperr->second.message;
                }

                auto app42err = rs.getErrors().find(Impl::App42);
                if ( app42err != rs.getErrors().end())
                {
                    app42ErrorCode = app42err->second.code;
                    errorDetails = app42err->second.message;
                }
            }

            Error::Error(entityx::Entity::Id a_id, std::string r)
                : id(a_id)
                , httpErrorCode(0)
                , app42ErrorCode(0)
                , errorMessage("timeout")
                , errorDetails(r)
            {
            }

        } // namespace Events
	} // namespace LocalData
}  // namespace WkCocos
