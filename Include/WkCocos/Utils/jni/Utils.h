#ifndef __UTILS_JNI_UTILS_H_
#define __UTILS_JNI_UTILS_H_

#define JAVA_PACKAGE "com/gameparkstudio/wkcocos/lib"

#include <string>
namespace WkCocos
{
	namespace Utils
	{
		namespace jni
		{
			class Utils
			{
			public:

				static std::string getVersionName();
			};
		} // nmaepsace Utils
	} // namespace jni
} // namespace WkCocos

#endif