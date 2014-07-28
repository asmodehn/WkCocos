#ifndef __WKCOCOS_APP42_SETUP_H__
#define __WKCOCOS_APP42_SETUP_H__

//dependencies headers
#include "Common/App42API.h"

namespace WkCocos
{
	namespace App42
	{
		class Setup
		{
		public:
			Setup(std::string APP_KEY, std::string SECRET_KEY);
			~Setup();



		};

	}// namespace App42
} //namespace WkCocos

#endif //__WKCOCOS_APP42_SETUP_H__
