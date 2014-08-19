#ifndef __WKCOCOS_TOOLBOX_H__
#define __WKCOCOS_TOOLBOX_H__

#include "cocos2d.h"

namespace WkCocos
{
	/**
	* Tool box class.
	* Set of useful static function.
	*/
	class ToolBox
	{
	public:
		/**
		* Default destructor
		*/
		virtual ~ToolBox();

		/**
		* Transform a int value to text
		* @param value Value to transform to text
		* @return A string representing the value.
		*/
		static std::string itoa(int value);

		/**
		* small extension on fopen, which creates intermediate directories if needed
		*/
		static FILE* FOpen(std::string name, std::string mode);

		/**
		* Implementation ( from VS C++ ) of std::stoul for Android, who doesnt have it ( no C99 function )
		*/
		static unsigned long stoul(const std::string& _Str, size_t *_Idx = 0, int _Base = 10);


		///Get Current Time
		static struct tm getLocalTime();

		///Get Current Time
		static struct tm getUTCTime();

	private:
		/**
		* Default constructor.
		* Static class, no need to create object.
		*/
		ToolBox();

	};
}//namespace WkCocos

#endif //__WKCOCOS_TOOLBOX_H__