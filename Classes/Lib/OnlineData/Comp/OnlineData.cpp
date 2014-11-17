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
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_userid(userid)
				, m_passwd(passwd)
				, m_email(email)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

						CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
						CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

						if (userdata->isSuccess)
						{//if creation succeed then login.
							cb(userdata);
						}
						else// if creation failed, 
						{
							CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
							CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
							CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
							CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

							cb(userdata);
						}
					}
					done = true;
					//userdata is deleted by App42SDK
				};
			}

			Login::Login(std::string userid, std::string passwd, std::function<void(::App42::App42UserResponse*)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_userid(userid)
				, m_passwd(passwd)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

						CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
						CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

						if (userdata->isSuccess)
						{//if login succeed then login.
							cb(userdata);
						}
						else// if login failed, 
						{
							CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
							CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
							CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
							CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

							cb(userdata);
						}
					}
					done = true;
					//userdata is deleted by App42SDK
				};
			}

			UpdateUserData::UpdateUserData(std::string userid, std::string collection, std::string docId, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_userid(userid)
				, m_collection(collection)
				, m_docid(docId)
				, m_user_data(user_data)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

						CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

						cb(userdata);
					}
					done = true; 
				};
			}

			InsertUserData::InsertUserData(std::string userid, std::string collection, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_userid(userid)
				, m_collection(collection)
				, m_user_data(user_data)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

						CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

						cb(userdata);
					}
					done = true;
				};
			}

			LoadUserData::LoadUserData(std::string userid, std::string collection, std::function<void(::App42::App42UserResponse*)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_userid(userid)
				, m_collection(collection)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);
						CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

						cb(userdata);
					}
					done = true;
				};
			}

			GetUsersKeyValue::GetUsersKeyValue(std::string collection, std::string key, int value, int quantity, int offset, std::function<void(std::map<std::string, std::string>, int)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_collection(collection)
				, m_key(key)
				, m_value(value)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
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
							CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
							CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
							CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
							CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
							cb(doc, 0);
						}
					}
					done = true;
				};
			}

			GetUsersFromTo::GetUsersFromTo(std::string collection, std::string key, int from, int to, int quantity, int offset, std::function<void(std::map<std::string, std::string>, int)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_collection(collection)
				, m_key(key)
				, m_from(from)
				, m_to(to)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
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
						else if (404 == userdata->httpErrorCode && 2608 == userdata->appErrorCode)
						{
							// emulate no user found
							// game logic needs to know when request failed.
							cb(doc, 0);
						}
						else// if request failed,
						{
							CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
							CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
							CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
							CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
							cb(doc, 0);
						}
					}
					done = true;
				};
			}

			ServerTime::ServerTime(std::function<void(std::string)> callback)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42TimerResponse* userdata = static_cast<::App42::App42TimerResponse*>(data);

						if (userdata->isSuccess)
						{
							//event_emitter->emit<Events::ServerTime>(userdata->app42Timer.currentTime);
							callback(userdata->app42Timer.currentTime);
						}
					}
					done = true;
				};
			}

			AllDocsPaging::AllDocsPaging(std::string collection, int quantity, int offset, std::function<void(std::vector<std::map<std::string, std::string>>)> cb)
				: in_progress(false)
				, done(false)
				, timeout(false)
				, life_time(0)
				, m_collection(collection)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data)
				{
					if (!timeout)
					{
						::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
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

							cb(table);
						}
						else
						{
							CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
							CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
							CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
							CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
							cb(table);
						}
					}
					done = true;
				};
			}

		}

	}//namespace OnlineData
} //namespace WkCocos