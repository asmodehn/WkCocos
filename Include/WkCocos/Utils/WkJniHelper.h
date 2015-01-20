#ifndef __UTILS_WKJNIHELPER_H_
#define __UTILS_WKJNIHELPER_H_

#include "cocos/cocos2d.h"

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>

#include "platform/android/jni/JniHelper.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO

#endif

#define JAVA_PACKAGE "com/gameparkstudio/wkcocos/lib"

#include <string>

namespace WkCocos
{
    //friend forward declaration
    namespace PushNotifications{
        class PushNotifications;
    }

	namespace Utils
	{
	    /**
	    * An internal Helper class to provide easy access to java methods
	    */
        class WkJniHelper
        {
        public:

            static std::string getVersionName();

			static void openURL(std::string url);
			static void showAd(cocos2d::Vec2 pos);
			static void hideAd();

        private:
            friend class WkCocos::PushNotifications::PushNotifications;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            static jobject getPushNotificationManager();

            static jobject getAppMainActivity();

            static jobject getInstance();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//??
#endif

        };
	} // nmaepsace Utils
} // namespace WkCocos

#endif // __UTILS_WKJNIHELPER_H_
