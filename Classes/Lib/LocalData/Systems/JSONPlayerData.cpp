#include "WkCocos/LocalData/Systems/JSONPlayerData.h"
#include "WkCocos/LocalData/Events/Error.h"
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
			void JSONPlayerData::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::File> file;
				entityx::ptr<Comp::PlayerData_v1> playerData;
				for (auto entity : es->entities_with_components(file, playerData))
				{
					rapidjson::Document doc;
					doc.Parse<0>(file->m_contents.c_str());
					if (doc.HasParseError()) {
						CCLOG("GetParseError %s\n", doc.GetParseError());
						//callback empty on invalid json
						if (playerData->m_load_cb)
						{
							playerData->m_load_cb("");
						}
					}
					else if (playerData->m_load_cb) //if we want to load data
					{
						if (doc.HasMember(playerData->m_name.c_str()))
						{
							rapidjson::Value& playerdatavalue = doc[playerData->m_name.c_str()];
							//writing json structure in a string
							rapidjson::StringBuffer strbuf;
							rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
							playerdatavalue.Accept(writer);
							//passing the string to the callback
							playerData->m_data = strbuf.GetString();
							playerData->m_load_cb(playerData->m_data);
							
						}
						else
						{
							playerData->m_load_cb("");
						}
					}
					else //write request
					{
						//clearing original saved data
						doc.SetObject();

						rapidjson::Document data;
						data.Parse<0>(playerData->m_data.c_str());
						if (data.HasParseError()) {
							CCLOG("GetParseError %s\n", data.GetParseError());
							//signal error
							events->emit<Events::Error>(entity);
							//WHAT TO DO ?
						}
						else
						{
							rapidjson::Value vholder;
							vholder.SetObject();
							for (auto dm = data.MemberonBegin(); dm != data.MemberonEnd(); ++dm)
							{
								vholder.AddMember(dm->name, dm->value, doc.GetAllocator());
							}
							doc.AddMember(playerData->m_name.c_str(), vholder, doc.GetAllocator());

							//TMP debug
							rapidjson::StringBuffer strbuf;
							rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
							doc.Accept(writer);

							file->m_contents = strbuf.GetString();
							CCLOG("new contents :  %s", file->m_contents.c_str());
						}
					}

					//we finished working with this component.
					entity.remove<Comp::PlayerData_v1>();
				}
			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos


