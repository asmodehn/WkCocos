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
		int PushNotifications::schedule(int id, long when, std::string title, std::string message)
		{
			int scheduled = 0;
			cocos2d::JniMethodInfo j_getInstanceMI;
			cocos2d::JniMethodInfo j_signInMI;
			CCLOG("Calling com/gameparkstudio/wkcocos/lib/PushNotificationsManager/getInstance()Lcom/gameparkstudio/wkcocos/lib/PushNotificationsManager");
			if (cocos2d::JniHelper::getStaticMethodInfo(j_getInstanceMI, "com/gameparkstudio/wkcocos/lib/PushNotificationsManager", "getInstance", "()Lcom/gameparkstudio/wkcocos/lib/PushNotificationsManager;"))
			{
				jobject instance = j_getInstanceMI.env->CallStaticObjectMethod(j_getInstanceMI.classID, j_getInstanceMI.methodID);
				CCLOG("Calling com/gameparkstudio/wkcocos/lib/PushNotificationsManager/schedule(JLjava/lang/String;Ljava/lang/String;)I");
				if (instance && cocos2d::JniHelper::getMethodInfo(j_signInMI, "com/gameparkstudio/wkcocos/lib/PushNotificationsManager", "schedule", "(IJLjava/lang/String;Ljava/lang/String;)V"))
				{
					//building arguments
					jint jid(id);
					jlong jwhen(when);
					jstring jtitle = cocos2d::JniHelper::string2jstring(title.c_str());
					jstring jmessage = cocos2d::JniHelper::string2jstring(message.c_str());

					scheduled = j_signInMI.env->CallIntMethod(instance, j_signInMI.methodID, jid, jwhen, jtitle, jmessage);

					j_getInstanceMI.env->DeleteLocalRef(jtitle);
					j_getInstanceMI.env->DeleteLocalRef(jmessage);

				}
				j_getInstanceMI.env->DeleteLocalRef(j_getInstanceMI.classID);
				j_getInstanceMI.env->DeleteLocalRef(instance);
			}
			return scheduled;
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		//PN on Win32 (HOW TO ?)

		int PushNotifications::schedule(long when, std::string title, std::string message)
		{
			return 0;
			//TODO ( REST calls ?? )
		}
#endif

	} // namespace PushNotifications
} // namespace WkCocos
