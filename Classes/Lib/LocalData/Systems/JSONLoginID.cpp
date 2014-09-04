#include "WkCocos/LocalData/Systems/JSONLoginID.h"
#include "WkCocos/LocalData/Comp/LocalData.h"

//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Systems
		{
			/**
			* This function will write into json file
			*/
			void JSONLoginID::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::File> file;
				entityx::ptr<Comp::LoginID_v1> loginid;
				for (auto entity : es->entities_with_components(file, loginid))
				{
					if (!entity.component<Comp::Read>())
					{
						rapidjson::Document doc;
						doc.Parse<0>(file->getContents().c_str());
						if (doc.HasParseError()) {
							CCLOG("GetParseError %s\n", doc.GetParseError());
							//callback empty on invalid json
							if (loginid->m_load_cb)
							{
								loginid->m_load_cb("", "");
							}
						}
						else if (loginid->m_load_cb)//if we want to load data
						{
							rapidjson::Value& loginvalue = doc[loginid->m_name.c_str()];
							if (!loginvalue.IsNull()){
								if (loginvalue.HasMember("user"))
								{
									loginid->m_user = loginvalue["user"].GetString();
								}
								if (loginvalue.HasMember("passwd"))
								{
									loginid->m_passwd = loginvalue["passwd"].GetString();
								}
							}
							loginid->m_load_cb(loginid->m_user, loginid->m_passwd);
						}
						else //write request
						{
							// must pass an allocator when the object may need to allocate memory
							rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

							//if we already have a login value in the file, we need to replace it
							if (doc.HasMember(loginid->m_name.c_str()))
							{
								rapidjson::Value& vholder = doc[loginid->m_name.c_str()];
								if (vholder.HasMember("user"))
								{
									vholder["user"] = loginid->m_user.c_str();
								}
								if (vholder.HasMember("passwd"))
								{
									vholder["passwd"] = loginid->m_passwd.c_str();
								}
							}
							else //new login id
							{
								rapidjson::Value vholder;
								vholder.SetObject();
								vholder.AddMember("user", loginid->m_user.c_str(), allocator);
								vholder.AddMember("passwd", loginid->m_passwd.c_str(), allocator);
								doc.AddMember(loginid->m_name.c_str(), vholder, allocator);
							}

							//TMP debug
							rapidjson::StringBuffer strbuf;
							rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
							doc.Accept(writer);

						file->setContents(strbuf.GetString());
						//CCLOG("new contents :  %s", file->getContents().c_str());
						}

						//we finished working with this component.
						entity.remove<Comp::LoginID_v1>();
					}
				}
			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos


