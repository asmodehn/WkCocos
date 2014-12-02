#include "WkCocos/OnlineData/Comp/OnlineData.h"
#include "WkCocos/OnlineData/Events/PlayersList.h"
//#include "WkCocos/OnlineData/Events/ServerTime.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Comp
		{
			Create::Create(std::string userid, std::string passwd, std::string email, std::function<void(RequestStatus rs)> cb)
				: m_userid(userid)
				, m_passwd(passwd)
				, m_email(email)
				, m_cb(cb)
			{

			}

			Login::Login(std::string userid, std::string passwd, std::function<void(RequestStatus rs, std::string)> cb)
				: m_userid(userid)
				, m_passwd(passwd)
				, m_cb(cb)
			{

			}

			UpdateUserData::UpdateUserData(std::string userid, std::string collection, std::string docId, std::string user_data, std::function<void(std::string, std::string, std::string)> cb)
				: m_userid(userid)
				, m_collection(collection)
				, m_docid(docId)
				, m_user_data(user_data)
				, m_cb(cb)
			{

			}

			InsertUserData::InsertUserData(std::string userid, std::string collection, std::string user_data, std::function<void(std::string, std::string, std::string)> cb)
				: m_userid(userid)
				, m_collection(collection)
				, m_user_data(user_data)
				, m_cb(cb)
			{

			}

			LoadUserData::LoadUserData(std::string userid, std::string collection, std::function<void(RequestStatus rs,std::string, std::vector<std::string>)> cb)
				: m_userid(userid)
				, m_collection(collection)
				, m_cb(cb)
			{

			}

			GetUsersKeyValue::GetUsersKeyValue(std::string collection, std::string key, int value, int quantity, int offset)
				: m_collection(collection)
				, m_key(key)
				, m_value(value)
				, m_quantity(quantity)
				, m_offset(offset)
			{

			}

			GetUsersFromTo::GetUsersFromTo(std::string collection, std::string key, int from, int to, int quantity, int offset)
				: m_collection(collection)
				, m_key(key)
				, m_from(from)
				, m_to(to)
				, m_quantity(quantity)
				, m_offset(offset)
			{

			}

			ServerTime::ServerTime(std::function<void(std::string)> callback) :m_cb(callback) {}

			AllDocsPaging::AllDocsPaging(std::string collection, int quantity, int offset)
				: m_collection(collection)
				, m_quantity(quantity)
				, m_offset(offset)
			{

			}

			ProgressUpdate::ProgressUpdate() : in_progress(false), life_time(0) { }

		}

	}//namespace OnlineData
} //namespace WkCocos
