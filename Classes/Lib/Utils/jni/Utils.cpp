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

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

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

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				//NOT IMPLEMENTED
				version = "0.0.0";
#endif
				return version;
			}
		}//namespace jni
	}
}

