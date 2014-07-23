#ifndef __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
#define __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

#include "cocos/cocos2d.h"
#include "WkCocos/Utils/ToolBox.h"

#include <fstream>

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Comp
		{
			struct Create : entityx::Component<Create>
			{
				Create(std::string userid, std::string passwd, std::string email, std::function<void(App42UserResponse*)> cb);
				
				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_passwd;
				std::string m_email;
				std::function<void(void*)> m_cb;

			};

			struct Login : entityx::Component<Login>
			{
				Login(std::string userid, std::string passwd, std::function<void(App42UserResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_passwd;
				std::function<void(void*)> m_cb;

			};

			struct SaveUserData : entityx::Component<SaveUserData>
			{
				SaveUserData(std::string userid, std::string user_data, std::function<void(App42UserResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_user_data;
				std::function<void(void*)> m_cb;
			};

			struct LoadUserData : entityx::Component<LoadUserData>
			{
				LoadUserData(std::string userid, std::function<void(App42UserResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_user_data;
				std::function<void(void*)> m_cb;
			};

			/*
			struct SavePublic : entityx::Component<SavePublic>
			{
				SavePublic(std::string userid, std::string public_data, std::function<void(App42StorageResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_public_data;
				std::function<void(void*)> m_cb;
				std::string m_suffix = "_public";
			};

			struct LoadPublic : entityx::Component<LoadPublic>
			{
				LoadPublic(std::string userid, std::function<void(App42StorageResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_public_data;
				std::function<void(void*)> m_cb;
				std::string m_suffix = "_public";
			};
			*/

		}//namespace Comp
	}//namespace OnlineData
}//namespace WkCocos

#endif // __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
