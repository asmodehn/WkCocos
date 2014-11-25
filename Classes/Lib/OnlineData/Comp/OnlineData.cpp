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
			Create::Create(std::string userid, std::string passwd, std::string email, std::function<void(::App42::App42UserResponse*)> cb)
				: m_userid(userid)
				, m_passwd(passwd)
				, m_email(email)
			{
				m_cb = [=](void* data)
				{
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					cb(userdata);

				};
			}

			Login::Login(std::string userid, std::string passwd, std::function<void(::App42::App42UserResponse*)> cb)
				: m_userid(userid)
				, m_passwd(passwd)
			{
				m_cb = [=](void* data)
				{
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					cb(userdata);

				};
			}

			UpdateUserData::UpdateUserData(std::string userid, std::string collection, std::string docId, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: m_userid(userid)
				, m_collection(collection)
				, m_docid(docId)
				, m_user_data(user_data)
			{
				m_cb = [=](void* data)
				{
                    ::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

                    CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

                    CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
                    CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
                    CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
                    CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

                    cb(userdata);

				};
			}

			InsertUserData::InsertUserData(std::string userid, std::string collection, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: m_userid(userid)
				, m_collection(collection)
				, m_user_data(user_data)
			{
				m_cb = [=](void* data)
				{
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					cb(userdata);

				};
			}

			LoadUserData::LoadUserData(std::string userid, std::string collection, /*std::function<void(::App42::App42UserResponse*)> cb*/ std::function<void(std::string, std::vector<std::string>)> callback)
				: m_userid(userid)
				, m_collection(collection)
				, m_cb(callback)
			{
			/*	m_cb = [=](void* data)
				{
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);
	
					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					cb(userdata);

				};*/
			}

			GetUsersKeyValue::GetUsersKeyValue(std::string collection, std::string key, int value, int quantity, int offset, std::function<void(std::map<std::string, std::string>, int)> cb)
				: m_collection(collection)
				, m_key(key)
				, m_value(value)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					std::map<std::string, std::string> doc;
					if (userdata->isSuccess)
					{
						auto firstStorage = userdata->storages.begin();
						auto jsonDocArray = firstStorage->jsonDocArray;
						for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
						{
							doc[it->getOwner()] = it->getJsonDoc();
						}
						cb(doc, (int)firstStorage->recordCount);
					}
					else
					{
						cb(doc, 0);
					}

				};
			}

			GetUsersFromTo::GetUsersFromTo(std::string collection, std::string key, int from, int to, int quantity, int offset, std::function<void(std::map<std::string, std::string>, int)> cb)
				: m_collection(collection)
				, m_key(key)
				, m_from(from)
				, m_to(to)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					std::map<std::string, std::string> doc;
					if (userdata->isSuccess)
					{
						auto firstStorage = userdata->storages.begin();
						auto jsonDocArray = firstStorage->jsonDocArray;
						for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
						{
							doc[it->getOwner()] = it->getJsonDoc();
						}
						cb(doc, (int)firstStorage->recordCount);
					}
					else
					{
						cb(doc, 0);
					}

				};
			}

			ServerTime::ServerTime(std::function<void(std::string)> callback)
			{
				m_cb = [=](void* data)
				{
					::App42::App42TimerResponse* userdata = static_cast<::App42::App42TimerResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					if (userdata->isSuccess)
					{
						callback(userdata->app42Timer.currentTime);
					}
					else
					{
						callback("1900-01-00T00:00:00.000");
					}

				};
			}

			AllDocsPaging::AllDocsPaging(std::string collection, int quantity, int offset, std::function<void(std::vector<std::map<std::string, std::string>>)> cb)
				: m_collection(collection)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
					CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
					CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
					CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

					std::vector<std::map<std::string, std::string>> table;
					if (userdata->isSuccess)
					{
						std::map<std::string, std::string> line;
						auto jsonDocArray = userdata->storages.begin()->jsonDocArray;
						for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
						{
							line["Document_ID"] = it->getDocId();
							line["JSON_Document"] = it->getJsonDoc();
							line["Owner"] = it->getOwner();
							line["Created_On"] = it->getCreatedAt();
							line["Updated_On"] = it->getUpdatedAt();
							table.push_back(line);
						}
					}
					cb(table);

				};
			}

			ProgressUpdate::ProgressUpdate() : in_progress(false), life_time(0) { }

		}

	}//namespace OnlineData
} //namespace WkCocos
