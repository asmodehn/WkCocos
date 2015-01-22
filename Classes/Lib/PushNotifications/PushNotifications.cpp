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

#include "WkCocos/Utils/WkJniHelper.h"

namespace WkCocos
{
	namespace PushNotifications
	{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		void PushNotifications::schedule(int id, long when, std::string title, std::string message)
		{
			cocos2d::JniMethodInfo j_scheduleMI;
			jobject instance = WkCocos::Utils::WkJniHelper::getPushNotificationManager();
			std::string clss = std::string(PACKAGE_NAME_JNI) + "/PushNotificationsManager";
            CCLOG("Calling %s/schedule(JLjava/lang/String;Ljava/lang/String;)I", clss.c_str());
            if (instance && cocos2d::JniHelper::getMethodInfo(j_scheduleMI, clss.c_str(), "schedule", "(IJLjava/lang/String;Ljava/lang/String;)V"))
            {
                //building arguments
                jint jid(id);
                jlong jwhen(when);
                jstring jtitle = cocos2d::JniHelper::string2jstring(title.c_str());
                jstring jmessage = cocos2d::JniHelper::string2jstring(message.c_str());

                j_scheduleMI.env->CallIntMethod(instance, j_scheduleMI.methodID, jid, jwhen, jtitle, jmessage);

                j_scheduleMI.env->DeleteLocalRef(jtitle);
                j_scheduleMI.env->DeleteLocalRef(jmessage);

            }
            j_scheduleMI.env->DeleteLocalRef(instance);
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

		//PN on Win32 (HOW TO ?)

		void PushNotifications::schedule(int id, long when, std::string title, std::string message)
		{

			//TODO ( REST calls ?? )
		}
#endif

	} // namespace PushNotifications
} // namespace WkCocos
