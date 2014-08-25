#ifndef __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
#define __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

#include "cocos/cocos2d.h"
//#include "WkCocos/Utils/ToolBox.h"

#include <fstream>

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Comp
		{
			struct Create : entityx::Component<Create>
			{
				Create(std::string userid, std::string passwd, std::string email, std::function<void(::App42::App42UserResponse*)> cb);
				
				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_passwd;
				std::string m_email;
				std::function<void(void*)> m_cb;

			};

			struct Login : entityx::Component<Login>
			{
				Login(std::string userid, std::string passwd, std::function<void(::App42::App42UserResponse*)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_passwd;
				std::function<void(void*)> m_cb;

			};

			struct DeleteUserData : entityx::Component<DeleteUserData>
			{
				DeleteUserData(std::string userid, std::string m_collection, std::string user_data, std::function<void(std::string)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_collection;
				std::string m_user_data;
				std::function<void(void*)> m_dummy_cb;

			};
			
			struct SaveUserData : entityx::Component<SaveUserData>
			{
				SaveUserData(std::string userid, std::string m_collection, std::string user_data, std::function<void(std::string)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_collection;
				std::string m_user_data;
				std::function<void(void*)> m_cb;
				std::function<void(void*)> m_dummy_cb;

			};

			struct LoadUserData : entityx::Component<LoadUserData>
			{
				LoadUserData(std::string userid, std::string m_collection, std::function<void(std::string)> cb);

				bool in_progress;
				bool done;
				std::string m_userid;
				std::string m_collection;
				//std::string m_user_data;
				std::function<void(void*)> m_cb;

			};

			struct GetAllUsers : entityx::Component<GetAllUsers>
			{
				GetAllUsers(entityx::ptr<entityx::EventManager> event_emitter);

				bool in_progress;
				bool done;
				std::function<void(void*)> m_cb;

			};

		}//namespace Comp
	}//namespace OnlineData
}//namespace WkCocos

#endif // __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
