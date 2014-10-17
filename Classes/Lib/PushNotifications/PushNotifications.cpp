#include "WkCocos/PushNotifications/PushNotifications.h"
#include "cocos/base/CCPlatformConfig.h"
#include "cocos/base/CCConsole.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Google Play Distribution on Android device ( Java calls )
#include <jni.h>

#include "platform/android/jni/JniHelper.h"

#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

namespace WkCocos
{
	namespace PushNotifications
	{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		bool PushNotifications::schedule(long when, std::string title, std::string message, int icon)
		{
			cocos2d::JniMethodInfo j_getInstanceMI;
			cocos2d::JniMethodInfo j_signInMI;
			CCLOG("Calling com/gameparkstudio/wkcocos/lib/PushNotificationsManager/getInstance()Lcom/gameparkstudio/wkcocos/lib/PushNotificationsManager");
			if (cocos2d::JniHelper::getStaticMethodInfo(j_getInstanceMI, "com/gameparkstudio/wkcocos/lib/PushNotificationsManager", "getInstance", "()Lcom/gameparkstudio/wkcocos/lib/PushNotificationsManager;"))
			{
				jobject instance = j_getInstanceMI.env->CallStaticObjectMethod(j_getInstanceMI.classID, j_getInstanceMI.methodID);
				CCLOG("Calling com/gameparkstudio/wkcocos/lib/PushNotificationsManager/schedule(JLjava/lang/String;Ljava/lang/String;I)Z");
				if (instance && cocos2d::JniHelper::getMethodInfo(j_signInMI, "com/gameparkstudio/wkcocos/lib/PushNotificationsManager", "schedule", "(JLjava/lang/String;Ljava/lang/String;I)Z"))
				{
					//building arguments
					jlong jwhen(when);
					jint jicon(icon);
					jstring jtitle = cocos2d::JniHelper::string2jstring(title.c_str());
					jstring jmessage = cocos2d::JniHelper::string2jstring(message.c_str());

					bool scheduled = j_signInMI.env->CallBooleanMethod(instance, j_signInMI.methodID, jwhen, jtitle, jmessage, jicon);

					j_getInstanceMI.env->DeleteLocalRef(jtitle);
					j_getInstanceMI.env->DeleteLocalRef(jmessage);

					return scheduled;
				}
				j_getInstanceMI.env->DeleteLocalRef(j_getInstanceMI.classID);
				j_getInstanceMI.env->DeleteLocalRef(instance);
			}
			else
			{
				return false;
			}
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		//PN on Win32 (HOW TO ?)

		bool PushNotifications::schedule(long when, std::string title, std::string message, int icon)
		{
			return false;
			//TODO ( REST calls ?? )
		}
#endif

	} // namespace PushNotifications
} // namespace WkCocos
