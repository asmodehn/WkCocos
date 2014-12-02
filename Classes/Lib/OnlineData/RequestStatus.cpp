#include "WkCocos/OnlineData/RequestStatus.h"

namespace WkCocos
{
	namespace OnlineData
	{
        std::ostream& operator<<(std::ostream& os, const ImplError& ie)
        {
            os << " ERROR Code " << ie.code << " Msg " << ie.message ;
            return os;
        }


        std::ostream& operator<<(std::ostream& os, const RequestStatus& rs)
        {
            for ( auto err : rs.getErrors() )
            {
                if ( err.first == Impl::HTTP )
                {
                    os << "HTTP";
                }
                else if(err.first == Impl::App42)
                {
                    os << "App42";
                }
                os << " : " << err.second << std::endl;
            }
        }


	} // namespace LocalData
}  // namespace WkCocos
