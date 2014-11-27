#include "WkCocos/Utils/jni/Utils.h"

#include "cocos/cocos2d.h"

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//UUID generation on Android device ( Java calls )
#include <jni.h>

#include "platform/android/jni/JniHelper.h"

#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#include "WkPlatform_WkCocos.h"

#endif

namespace WkCocos
{
	namespace Utils
	{
		namespace jni
		{
			std::string Utils::getVersionName()
			{
				std::string version = "";

				//platform detection based on cocos
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				//getting apk version name ( from Android code )

				cocos2d::JniMethodInfo j_getVNameMI;
				std::string logstr = std::string("Calling ") + std::string(JAVA_PACKAGE) + std::string("/Utils/getVersionName()Ljava/lang/String;");
				CCLOG("%s",logstr.c_str());
				std::string UtilsStr = std::string(JAVA_PACKAGE) + std::string("/Utils");
				if (cocos2d::JniHelper::getStaticMethodInfo(j_getVNameMI, UtilsStr.c_str() , "getVersionName", "()Ljava/lang/String;"))
				{
					cocos2d::JniMethodInfo j_UUIDtoStringMI;
					jobject jvername = j_getVNameMI.env->CallStaticObjectMethod(j_getVNameMI.classID, j_getVNameMI.methodID);

					const char* jvernamecstr = j_getVNameMI.env->GetStringUTFChars((jstring)jvername, NULL);
					version.assign(jvernamecstr);
					j_getVNameMI.env->ReleaseStringUTFChars((jstring)jvername, jvernamecstr);

					j_getVNameMI.env->DeleteLocalRef(j_getVNameMI.classID);
					j_getVNameMI.env->DeleteLocalRef(jvername);
				}
#endif
				return version;
			}

			void Utils::openURL(std::string url)
            {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                cocos2d::JniMethodInfo minfo;

                if (cocos2d::JniHelper::getStaticMethodInfo(minfo, "com/gameparkstudio/wkcocos/lib/MainActivity", "openURL", "(Ljava/lang/String;)V")) {
                    jstring StringArg1 = minfo.env->NewStringUTF(url.c_str());
                    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
                    minfo.env->DeleteLocalRef(StringArg1);
                    minfo.env->DeleteLocalRef(minfo.classID);
                }
#else
                //TODO
#endif
            }

		}//namespace jni
	}
}

