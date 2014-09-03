#include "WkCocos/OnlineData/Comp/OnlineData.h"
#include "WkCocos/OnlineData/Events/PlayersList.h"
#include "WkCocos/OnlineData/Events/EnemyData.h"
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

			DeleteUserData::DeleteUserData(std::string userid, std::string collec, std::string user_data, std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
				, m_user_data(user_data)
			{
				m_dummy_cb = [=](void* data) {};
			}

			SaveUserData::SaveUserData(std::string userid, std::string collec, std::string user_data, std::function<void(std::string)> cb)
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
					CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

					if (userdata->isSuccess)
					{//if request succeed, we need to extract data from it
						rapidjson::Document doc;
						doc.Parse<0>(userdata->getBody().c_str());
						if (doc.HasParseError())
						{
							//if parse error (also empty string), we ignore existing data.
							cb("");
						}
						else if (doc.HasMember("data")) //TOFIX : data is not first level member of the response...
						{
							//TMP debug
							rapidjson::StringBuffer strbuf;
							rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
							doc["data"].Accept(writer);
							cb(strbuf.GetString());
						}
						else
						{
							cb("");
						}
						
					}
					else// if request failed, 
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

						cb("");
					}

					done = true;
					//userdata is deleted by App42SDK
				};
				m_dummy_cb = [=](void* data) {};
			}

			LoadUserData::LoadUserData(std::string userid, std::string collec, std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
				//, m_user_data("")
			{
				m_cb = [=](void* data) {
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
					CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

					if (userdata->isSuccess)
					{//if request succeed, we need to extract data from it
						rapidjson::Document doc;
						doc.Parse<0>(userdata->getBody().c_str());
						
						if (doc.HasParseError())
						{
							cb(""); // if parse error (also empty string), we ignore existing data.
						}
						else
						{
							if (doc.HasMember("app42"))
							{
								rapidjson::Value & temp = doc["app42"];
								temp = temp["response"];
								temp = temp["storage"];
								temp = temp["jsonDoc"];
								if (temp.Size())
								{
									temp = temp[temp.Size() - 1];
									if (temp.HasMember("data"))
									{
										rapidjson::StringBuffer strbuf;
										rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
										temp["data"].Accept(writer);
										cb(strbuf.GetString());
									}
									else
									{
										cb("");
									}
								}
								else
								{
									cb("");
								}
							}
							else
							{
								if (doc.Size())
								{
									rapidjson::Value & temp = doc[doc.Size() - 1];
									if (temp.HasMember("data"))
									{
										rapidjson::StringBuffer strbuf;
										rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
										temp["data"].Accept(writer);
										cb(strbuf.GetString());
									}
									else
									{
										cb("");
									}
								}
								else
								{
									cb("");
								}
							}
						}
					}
					else// if request failed, 
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);

						cb("");
					}

					done = true;
					//userdata is deleted by App42SDK
				};
			}

			LoadEnemyData::LoadEnemyData(std::string userid, std::string collec, entityx::ptr<entityx::EventManager> event_emitter)
				: in_progress(false)
				, done(false)
				, m_userid(userid)
				, m_collection(collec)
			{
				m_cb = [=](void* data) {
					::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);

					if (userdata->isSuccess)
					{//if request succeed, we need to extract data from it
						rapidjson::Document doc;
						doc.Parse<0>(userdata->getBody().c_str());

						if (!doc.HasParseError())
						{
							if (doc.HasMember("app42"))
							{
								rapidjson::Value & temp = doc["app42"];
								temp = temp["response"];
								temp = temp["storage"];
								temp = temp["jsonDoc"];
								if (temp.Size())
								{
									temp = temp[temp.Size() - 1];
									if (temp.HasMember("data"))
									{
										temp = temp["data"];
										temp = temp["currency"];
										event_emitter->emit<Events::EnemyData>(userid, temp["gold"].GetInt(), temp["gem"].GetInt(), true);
									}
									else
										event_emitter->emit<Events::EnemyData>(userid, 0, 0, false);
								}
								else
									event_emitter->emit<Events::EnemyData>(userid, 0, 0, false);
							}
							else
								event_emitter->emit<Events::EnemyData>(userid, 0, 0, false);
						}
						else
							event_emitter->emit<Events::EnemyData>(userid, 0, 0, false);

					}
					else// if request failed, 
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
						event_emitter->emit<Events::EnemyData>(userid, 0, 0, false);
					}

					done = true;
					//userdata is deleted by App42SDK
				};
			}

			GetAllUsers::GetAllUsers(entityx::ptr<entityx::EventManager> event_emitter)
				: in_progress(false)
				, done(false)

			{
				m_cb = [=](void* data) {
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					//CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
					//following line causes crash!
					//CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

					if (userdata->isSuccess)
					{
						rapidjson::Document doc;
						doc.Parse<0>(userdata->getBody().c_str());

						if (!doc.HasParseError())
						{
							if (doc.HasMember("app42"))
							{
								rapidjson::Value & temp = doc["app42"];
								temp = temp["response"];
								temp = temp["users"];

								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								temp["user"].Accept(writer);

								event_emitter->emit<Events::PlayersList>(strbuf.GetString());

							}
						}
					}
					else 
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
					}

					done = true;
					//userdata is deleted by App42SDK
				};
			}

			GetUsersWithDocs::GetUsersWithDocs(entityx::ptr<entityx::EventManager> event_emitter)
				: in_progress(false)
				, done(false)

			{
				m_cb = [=](void* data) {/*
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					//CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
					//following line causes crash!
					//CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

					if (userdata->isSuccess)
					{
						rapidjson::Document doc;
						doc.Parse<0>(userdata->getBody().c_str());

						if (!doc.HasParseError())
						{
							if (doc.HasMember("app42"))
							{
								rapidjson::Value & temp = doc["app42"];
								temp = temp["response"];
								temp = temp["users"];

								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								temp["user"].Accept(writer);

								event_emitter->emit<Events::PlayersList>(strbuf.GetString());

							}
						}
					}
					else
					{
						CCLOG("\nerrordetails:%s", userdata->errorDetails.c_str());
						CCLOG("\nerrorMessage:%s", userdata->errorMessage.c_str());
						CCLOG("\nappErrorCode:%d", userdata->appErrorCode);
						CCLOG("\nhttpErrorCode:%d", userdata->httpErrorCode);
					}

					done = true;
					//userdata is deleted by App42SDK*/
				};
			}

		}

	}//namespace OnlineData
} //namespace WkCocos

