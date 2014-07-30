#ifndef __WKCOCOS_UUID_H__
#define __WKCOCOS_UUID_H__

#include <string>

namespace WkCocos
{
	/**
	* UUID class.
	* to manage UUID in a portable way
	*/
	class UUID
	{
	public:

		static std::string create();
		
	};

}

#endif // __WKCOCOS_UUID_H__