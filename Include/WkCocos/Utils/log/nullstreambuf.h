#ifndef __WKCOCOS_UTILS_LOG_NULLSTREAMBUF_H__
#define __WKCOCOS_UTILS_LOG_NULLSTREAMBUF_H__

#include <streambuf>

namespace WkCocos
{

	namespace stdext
	{

		template <class cT, class traits = std::char_traits<cT> >
		class basic_nullstreambuf : public std::basic_streambuf<cT, traits>
		{
		public:
			basic_nullstreambuf();
			~basic_nullstreambuf();

			typename traits::int_type overflow(typename traits::int_type c)
			{
				return traits::not_eof(c); // indicate success
			}
		};


		template <class _CharT, class _Traits>
		basic_nullstreambuf<_CharT, _Traits>::basic_nullstreambuf()
			: std::basic_streambuf<_CharT, _Traits>()
		{
			}

		template <class _CharT, class _Traits>
		basic_nullstreambuf<_CharT, _Traits>::~basic_nullstreambuf()
		{
		}


	}



}//WkCocos

#endif //__WKCOCOS_UTILS_LOG_NULLSTREAMBUF_H__