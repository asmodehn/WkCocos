#include "WkCocos/Utils/UUID.h"

#include "cocos/cocos2d.h"

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <locale>
#include <codecvt>
//For UUID on windows
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//UUID generation on Android device ( Java calls )
#include <jni.h>

#include "platform/android/jni/JniHelper.h"

#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#endif

namespace WkCocos
{
	std::string UUID::create()
	{
		std::string uuid = "";

		//platform detection based on cocos
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//generating UUID ( from Android code )
		
		cocos2d::JniMethodInfo j_randomUUIDMI;
		CCLOG("Calling java/util/UUID/randomUUID()Ljava.util.UUID;");
		if (cocos2d::JniHelper::getStaticMethodInfo(j_randomUUIDMI, "java/util/UUID", "randomUUID", "()Ljava/util/UUID;"))
		{
			cocos2d::JniMethodInfo j_UUIDtoStringMI;
			jobject juuid = j_randomUUIDMI.env->CallStaticObjectMethod(j_randomUUIDMI.classID, j_randomUUIDMI.methodID);
			CCLOG("Calling java/util/UUID/toString()Ljava/lang/String;");
			if (juuid && cocos2d::JniHelper::getMethodInfo(j_UUIDtoStringMI, "java/util/UUID", "toString", "()Ljava/lang/String;"))
			{
				jstring uuidjstr = (jstring)j_UUIDtoStringMI.env->CallObjectMethod(juuid, j_UUIDtoStringMI.methodID);
				const char* uuidcstr = j_UUIDtoStringMI.env->GetStringUTFChars((jstring)uuidjstr, NULL);
				uuid.assign(uuidcstr);
				j_UUIDtoStringMI.env->ReleaseStringUTFChars(uuidjstr, uuidcstr);
				//Here uuid variable is setup with a UUID string.
				j_UUIDtoStringMI.env->DeleteLocalRef(j_UUIDtoStringMI.classID);
				j_UUIDtoStringMI.env->DeleteLocalRef(uuidjstr);
			}
			j_randomUUIDMI.env->DeleteLocalRef(j_randomUUIDMI.classID);
			j_randomUUIDMI.env->DeleteLocalRef(juuid);
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		///WIN32 only code !!! from http://src.chromium.org/svn/trunk/src/net/base/keygen_handler_win.cc

		//generating UUID
		::UUID win32_uuid = { 0 };
		//std::wstring wuuid;
		RPC_CSTR rpc_string = NULL;

		if (RPC_S_OK == UuidCreate(&win32_uuid) && RPC_S_OK == UuidToString(&win32_uuid, &rpc_string))
		{

			//VS2013 implementation
			uuid.assign(reinterpret_cast<char*>(rpc_string));

			//VS2010 implementation
			// RPC_WSTR is unsigned short*.  wchar_t is a built-in type of Visual C++,
			// so the type cast is necessary.
			//wuuid.assign(reinterpret_cast<wchar_t*>(rpc_string));
			//RpcStringFree(&rpc_string);
			//uuid = std::wstring_convert<std::codecvt_utf8<wchar_t> >().to_bytes(wuuid);
		}
#endif

		return uuid;
	}
}




