#include "WkCocos/OnlineData/Comp/OnlineData.h"

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
						else if (doc.HasMember("data"))
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

			GetAllUsers::GetAllUsers(std::function<void(std::string)> cb)
				: in_progress(false)
				, done(false)

			{
				m_cb = [=](void* data) {
					::App42::App42UserResponse* userdata = static_cast<::App42::App42UserResponse*>(data);

					CCLOG("\ncode=%d...=%d", userdata->getCode(), userdata->isSuccess);
					CCLOG("\nResponse Body=%s", userdata->getBody().c_str());

					if (userdata->isSuccess)
					{
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
								temp = temp["users"];

								rapidjson::StringBuffer strbuf;
								rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
								temp["user"].Accept(writer);

								temp = temp["user"];

								if (temp.Size())
								{
									for (rapidjson::SizeType i = 0; i < temp.Size(); i++)
									{
										rapidjson::Value & userName = temp[i];
										CCLOG("user number %d is %s\n", i, userName["userName"].GetString());
										/*userName = userName["profile"];
										static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
										for (rapidjson::Value::ConstMemberIterator itr = userName.MemberonBegin(); itr != userName.MemberonEnd(); ++itr)
											CCLOG("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
										*/								
									}
								}

								cb(strbuf.GetString());
							}
							else
							{
								cb("");
							}
						}
					}
					else 
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

		}

	}//namespace OnlineData
} //namespace WkCocos

