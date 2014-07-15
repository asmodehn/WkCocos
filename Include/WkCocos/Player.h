#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/LocalData/LocalDataManager.h"

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
	/**
	* Hold the player information.
	* Meyers singleton as the player is there from the beginning, and should available everywhere.
	* we use CRTP design to allow inheritance from it.
	*/
	template<class T>
	class Player
	{
	public:
		static T& GetInstance()
		{
			//careful : this might not be thread safe ...
			m_bSingletonCreating = true;
			static T player;
			m_bSingletonCreating = false;

			return player;
		}

		/**
		* Delayed intialization.
		* Check if saved data exist, and if so it loads it
		*/
		bool Activate();

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);

	protected:
		Player();
		Player(const Player&);
		Player& operator=(const Player&);

		static bool m_bSingletonCreating;

		std::string m_user;
		std::string m_passwd;

		LocalData::LocalDataManager m_localdata;

	};

	// Static variable initialization
	template<class T>
	bool Player<T>::m_bSingletonCreating = false;

	//constructors
	template<class T>
	Player<T>::Player()
	{
	}

	template<class T>
	Player<T>::Player(const Player&)
	{
	}

	//operators
	template<class T>
	Player<T>& Player<T>::operator=(const Player&)
	{
		return *this;
	}

	//member methods
	template<class T>
	bool Player<T>::Activate()
	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player<T>::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");
		
		//tried to read existing data.
		m_localdata.loadLoginID([&](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				m_user = user;
				m_passwd = passwd;
			}
			else
			{
				//generate unique ID
				std::string user_prefix = "fake_";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				//generating UUID ( from Android code )
				std::string uuid = "";

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

						user = user_prefix + uuid;
						//generating password
						passwd = uuid;
					}
					j_randomUUIDMI.env->DeleteLocalRef(j_randomUUIDMI.classID);
					j_randomUUIDMI.env->DeleteLocalRef(juuid);
				}

				//generating password (deterministic way, so we can recover password later)
				//TODO


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

				///WIN32 only code !!! from http://src.chromium.org/svn/trunk/src/net/base/keygen_handler_win.cc

				//generating UUID
				UUID win32_uuid = { 0 };
				std::wstring wuuid;
				std::string uuid;
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

					user = user_prefix + uuid;//TMP + uuid;
					//generating password
					passwd = uuid;
				}

#endif
				if (user.length() > 0 && passwd.length() > 0)
				{
					m_user = user;
					m_passwd = passwd;
					//adding fake email on creation ( email is used for password recovery )
					//std::string email = user + "@fake.net";

					//BApp42ResponseDelegate * response = BApp42ResponseDelegate::create();

					//CCLOG("Requesting App42 User creation : %s ", user.c_str());
					//UserService::getInstance()->CreateUser(user, password, email, response, callfuncND_selector(BApp42ResponseDelegate::onCreateMeRequestCompleted));

					//return response->createMeRequestCompleted;
				}











				//store unique ID
				m_localdata.saveLoginID(m_user,m_passwd);
			}


		});

		


		return false;
	}

	template<class T>
	void Player<T>::Update(float deltatime)
	{
		m_localdata.update(deltatime);
	}

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
