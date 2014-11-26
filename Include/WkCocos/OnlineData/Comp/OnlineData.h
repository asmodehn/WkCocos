#ifndef __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
#define __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

#include "cocos/cocos2d.h"
//#include "WkCocos/Utils/ToolBox.h"

#include <fstream>

#define DB_NAME "PUBLIC"
#define TIMEOUT 10.0

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Comp
		{
			struct Create : entityx::Component<Create>
			{
				Create(std::string userid, std::string passwd, std::string email, std::function<void(void)> cb);

				std::string m_userid;
				std::string m_passwd;
				std::string m_email;
				std::function<void(void)> m_cb;

			};

			struct Login : entityx::Component<Login>
			{
				Login(std::string userid, std::string passwd, std::function<void(std::string)> cb);

				std::string m_userid;
				std::string m_passwd;
				std::function<void(std::string)> m_cb;

			};

			struct UpdateUserData : entityx::Component<UpdateUserData>
			{
				UpdateUserData(std::string userid, std::string collec, std::string docid, std::string user_data, std::function<void(std::string, std::string, std::string)> cb);

				std::string m_userid;
				std::string m_collection;
				std::string m_docid;
				std::string m_user_data;
				std::function<void(std::string, std::string, std::string)> m_cb;

			};

			struct InsertUserData : entityx::Component<InsertUserData>
			{
				InsertUserData(std::string userid, std::string m_collection, std::string user_data, std::function<void(std::string, std::string, std::string)> cb);

				std::string m_userid;
				std::string m_collection;
				std::string m_user_data;
				std::function<void(std::string, std::string, std::string)> m_cb;

			};

			struct LoadUserData : entityx::Component<LoadUserData>
			{
				LoadUserData(std::string userid, std::string m_collection, std::function<void(std::string, std::vector<std::string>)> cb);

				std::string m_userid;
				std::string m_collection;
				std::function<void(std::string, std::vector<std::string>)> m_cb;

			};

			struct GetUsersKeyValue : entityx::Component<GetUsersKeyValue>
			{
				GetUsersKeyValue(std::string collection, std::string key, int value, int quantity, int offset);

				std::string m_collection;
				std::string m_key;
				int m_value;
				int m_quantity;
				int m_offset;

			};

			struct GetUsersFromTo : entityx::Component<GetUsersFromTo>
			{
				GetUsersFromTo(std::string collection, std::string key, int from, int to, int quantity, int offset);

				std::string m_collection;
				std::string m_key;
				int m_from;
				int m_to;
				int m_quantity;
				int m_offset;

			};

			struct ServerTime : entityx::Component<ServerTime>
			{
				ServerTime(std::function<void(std::string)> callback);

				std::function<void(std::string)> m_cb;

			};

			struct AllDocsPaging : entityx::Component<AllDocsPaging>
			{
				AllDocsPaging(std::string collection, int quantity, int offset);

				std::string m_collection;
				int m_quantity;
				int m_offset;

			};

			struct ProgressUpdate : entityx::Component<ProgressUpdate>
			{
				ProgressUpdate();

				bool in_progress;
				double life_time;

			};

		}//namespace Comp
	}//namespace OnlineData
}//namespace WkCocos

#endif // __WKCOCOS_ONLINEDATA_COMP_ONLINEDATA_H__
