#ifndef __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__
#define __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__

#include <string>
#define PACKAGE_NAME_JNI "com/asmodehn/wkcocos/lib"

namespace WkCocos
{
	namespace PushNotifications
	{
		class PushNotifications
		{
		public:
			static void schedule(int id, long when, std::string title, std::string message);

		};

	}
}

#endif // __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__
