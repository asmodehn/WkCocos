#ifndef __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__
#define __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__

#include <string>

namespace WkCocos
{
	namespace PushNotifications
	{
		class PushNotifications
		{
		public:
			static bool schedule(long when, std::string title, std::string message);

		};

	}
}

#endif // __WKCOCOS_PUSHNOTIFICATIONS_PUSHNOTIFICATIONS_H__