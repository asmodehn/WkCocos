#ifndef __WKCOCOS_ONLINEDATA_REQUESTSTATUS_H__
#define __WKCOCOS_ONLINEDATA_REQUESTSTATUS_H__

#include "entityx/entityx.h"

#include <ostream>
#include <map>

namespace WkCocos
{
	namespace OnlineData
	{
	    typedef entityx::Entity::Id RequestID;

        enum class Status
        {
            REQUEST_SUCCESS,
            REQUEST_ERROR,
            REQUEST_TIMEOUT
        };

	    struct ImplError
	    {
	        ImplError( long errcode, std::string message)
	        : code(errcode)
	        , message(message)
	        {}

	        long code;
	        std::string message;

            friend std::ostream& operator<< ( std::ostream& os, const ImplError& ie );
        };

        enum class Impl{
            HTTP,
            App42,
            WkCocos
        };

	    class RequestStatus
	    {

        public :
            RequestStatus(RequestID id, Status s, const std::map<Impl,ImplError>& errorlist)
            : m_s(s)
            , m_errors(errorlist)
            {}

            RequestID getRequestId() const
            {
                return m_id;
            }
            Status getStatus() const
            {
                return m_s;
            }
            std::map<Impl,ImplError> getErrors() const
            {
                return m_errors;
            }

            friend std::ostream& operator<< ( std::ostream& os, const RequestStatus& rs );

        private:
            RequestID m_id;
            Status m_s;
            std::map<Impl,ImplError> m_errors;
	    };

	} // namespace OnlineData
} // namespace WkCocos

#endif // __WKCOCOS_ONLINEDATA_REQUESTSTATUS_H__
