#ifndef __WKCOCOS_UTILS_LOG_NULLOSTREAM_H__
#define __WKCOCOS_UTILS_LOG_NULLOSTREAM_H__

#include "WkCocos/Utils/log/nullstreambuf.h"

#include <ostream>

namespace WkCocos
{

	namespace stdext
	{

		template <class cT, class traits = std::char_traits<cT> >
		class basic_onullstream : public std::basic_ostream<cT, traits>
		{
		public:
			basic_onullstream() :
				std::basic_ios<cT, traits>(),
				std::basic_ostream<cT, traits>(0),
				m_sbuf()
			{
					this->init(&m_sbuf);
				}

		private:
			basic_nullstreambuf<cT, traits> m_sbuf;
		};

		typedef basic_onullstream<char> onullstream;
		typedef basic_onullstream<wchar_t> wonullstream;


	} //stdext

	extern stdext::onullstream cnull;
	extern stdext::wonullstream wcnull;


} //WkCocos




#endif //__WKCOCOS_UTILS_LOG_NULLOSTREAM_H__