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
				, m_userid(userid)
				, m_passwd(passwd)
				, m_email(email)
			{
				m_cb = [=](void* data) {
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

					done = true;
					//userdata is deleted by App42SDK
				};
			}

			Login::Login(std::string userid, std::string passwd, std::function<void(::App42::App42UserResponse*)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_passwd(passwd)
			{
				m_cb = [=](void* data) {
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

					done = true;
					//userdata is deleted by App42SDK
				};
			}

			UpdateUserData::UpdateUserData(std::string userid, std::string collec, std::string docid, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
				, m_docid(docid)
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

					done = true; 
				};
			}

			FindUserData::FindUserData(std::string userid, std::string collec, std::function<void(std::string)> update_cb, std::function<void()> insert_cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
			{
				m_cb = [=](void* data) {
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					if (userdata->isSuccess)
					{
						std::vector<::App42::JSONDocument> jsonDocArray = userdata->users.front().jsonDocArray;
						if (jsonDocArray.size())
							update_cb(jsonDocArray.back().getDocId());
						else
							insert_cb();
					}
					else// if request failed, 
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

						insert_cb();
					}

					done = true;
				};
			}

			InsertUserData::InsertUserData(std::string userid, std::string collec, std::string user_data, std::function<void(::App42::App42StorageResponse*)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
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

					done = true;
				};
			}

			LoadUserData::LoadUserData(std::string userid, std::string collec, std::function<void(::App42::App42UserResponse*)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
			{
				m_cb = [=](void* data) {
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);
					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);

					cb(userdata);

					done = true;
				};
			}

			GetUsersWithDocs::GetUsersWithDocs(std::string collec, std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)
				, m_collection(collec)
			{
				m_cb = [=](void* data) {
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
					if (userdata->isSuccess)
					{
						for (std::vector<::App42::App42Storage>::iterator it = userdata->storages.begin(); it != userdata->storages.end(); ++it)
						{
							if (it->collectionName == collec)
							{
								rapidjson::Document doc;
								rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
								doc.SetArray();
								for (std::vector<::App42::JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit)
								{
									//everything from the following is so horrible just because json is so rapid
									rapidjson::Value temp;
									temp.SetObject();
									rapidjson::Value owner;
									char ownerbuf[42]; //taken from actual user name length, fake_+uuid, also 42
									int ownerlen = sprintf(ownerbuf, "%s", iit->getOwner().c_str());
									owner.SetString(ownerbuf, ownerlen, allocator);
									memset(ownerbuf, 0, sizeof(ownerbuf));
									temp.AddMember("owner", owner, allocator);
									rapidjson::Value jsonDoc;
									//following limit is taken from App42_Cocos2DX_SDK commit 109c2c9 file HMAC_SHA1.h
									char jsonDocbuf[32768]; //we just can not have longer doc without crash in other part of app
									int jsonDoclen = sprintf(jsonDocbuf, "%s", iit->getJsonDoc().c_str()); 
									jsonDoc.SetString(jsonDocbuf, jsonDoclen, allocator); //should be added another way because this way i need to parse it later
									memset(jsonDocbuf, 0, sizeof(jsonDocbuf));
									temp.AddMember("jsonDoc", jsonDoc, allocator);
									doc.PushBack(temp, allocator);
								}
								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								doc.Accept(writer);
								//callback after parsing
								cb(strbuf.GetString());
							}
						}
					}
					else// if request failed,
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
					}

					done = true;
				};
			}

			GetUsersKeyValue::GetUsersKeyValue(std::string collec, std::string key, int value, int quantity, int offset, std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)
				, m_collection(collec)
				, m_key(key)
				, m_value(value)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data) {
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
					if (userdata->isSuccess)
					{
						for (std::vector<::App42::App42Storage>::iterator it = userdata->storages.begin(); it != userdata->storages.end(); ++it)
						{
							if (it->collectionName == collec)
							{
								rapidjson::Document doc;
								rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
								doc.SetArray();
								for (std::vector<::App42::JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit)
								{
									//everything from the following is so horrible just because json is so rapid
									rapidjson::Value temp;
									temp.SetObject();
									rapidjson::Value owner;
									char ownerbuf[42]; //taken from actual user name length, fake_+uuid, also 42
									int ownerlen = sprintf(ownerbuf, "%s", iit->getOwner().c_str());
									owner.SetString(ownerbuf, ownerlen, allocator);
									memset(ownerbuf, 0, sizeof(ownerbuf));
									temp.AddMember("owner", owner, allocator);
									rapidjson::Value jsonDoc;
									//following limit is taken from App42_Cocos2DX_SDK commit 109c2c9 file HMAC_SHA1.h
									char jsonDocbuf[32768]; //we just can not have longer doc without crash in other part of app
									int jsonDoclen = sprintf(jsonDocbuf, "%s", iit->getJsonDoc().c_str());
									jsonDoc.SetString(jsonDocbuf, jsonDoclen, allocator); //should be added another way because this way i need to parse it later
									memset(jsonDocbuf, 0, sizeof(jsonDocbuf));
									temp.AddMember("jsonDoc", jsonDoc, allocator);
									doc.PushBack(temp, allocator);
								}
								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								doc.Accept(writer);
								//callback after success
								cb(strbuf.GetString());
							}
						}
					}
					else// if request failed,
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
					}

					done = true;
				};
			}

			GetUsersFromTo::GetUsersFromTo(std::string collec, std::string key, int from, int to, int quantity, int offset, std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)
				, m_collection(collec)
				, m_key(key)
				, m_from(from)
				, m_to(to)
				, m_quantity(quantity)
				, m_offset(offset)
			{
				m_cb = [=](void* data) {
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
					if (userdata->isSuccess)
					{
						for (std::vector<::App42::App42Storage>::iterator it = userdata->storages.begin(); it != userdata->storages.end(); ++it)
						{
							if (it->collectionName == collec)
							{
								rapidjson::Document doc;
								rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
								doc.SetArray();
								for (std::vector<::App42::JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit)
								{
									//everything from the following is so horrible just because json is so rapid
									rapidjson::Value temp;
									temp.SetObject();
									rapidjson::Value owner;
									char ownerbuf[42]; //taken from actual user name length, fake_+uuid, also 42
									int ownerlen = sprintf(ownerbuf, "%s", iit->getOwner().c_str());
									owner.SetString(ownerbuf, ownerlen, allocator);
									memset(ownerbuf, 0, sizeof(ownerbuf));
									temp.AddMember("owner", owner, allocator);
									rapidjson::Value jsonDoc;
									//following limit is taken from App42_Cocos2DX_SDK commit 109c2c9 file HMAC_SHA1.h
									char jsonDocbuf[32768]; //we just can not have longer doc without crash in other part of app
									int jsonDoclen = sprintf(jsonDocbuf, "%s", iit->getJsonDoc().c_str());
									jsonDoc.SetString(jsonDocbuf, jsonDoclen, allocator); //should be added another way because this way i need to parse it later
									memset(jsonDocbuf, 0, sizeof(jsonDocbuf));
									temp.AddMember("jsonDoc", jsonDoc, allocator);
									doc.PushBack(temp, allocator);
								}
								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								doc.Accept(writer);
								//callback after success
								cb(strbuf.GetString());
							}
						}
					}
					else// if request failed,
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
					}

					done = true;
				};
			}

			ServerTime::ServerTime(std::function<void(std::string)> callback)
				: in_progress(false)
				, done(false)
			{
				m_cb = [=](void* data)
				{
					::App42::App42TimerResponse* userdata = static_cast<::App42::App42TimerResponse*>(data);

					if (userdata->isSuccess)
					{
						//event_emitter->emit<Events::ServerTime>(userdata->app42Timer.currentTime);
						callback(userdata->app42Timer.currentTime);
					}
				};
			}

		}

	}//namespace OnlineData
} //namespace WkCocos