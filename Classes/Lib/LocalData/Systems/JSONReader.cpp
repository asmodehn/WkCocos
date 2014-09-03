#include "WkCocos/LocalData/Systems/JSONReader.h"

#include "WkCocos/LocalData/Comp/LocalData.h"
#include "WkCocos/LocalData/Events/Error.h"

//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/prettywriter.h"     // for stringify JSON
#include "json/stringbuffer.h"       // wrapper of string for prettywriter as output

#include "cocos2d.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Systems
		{
			/**
			* This function will write into json file
			*/
			void JSONReader::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::File> file;
				entityx::ptr<Comp::Read> read;
				for (auto entity : es->entities_with_components(file, read))
				{
					rapidjson::Document doc;

					bool fileread = file->read();

					//we try to read
					if (fileread && file->m_contents != "")
					{
						doc.Parse<0>(file->m_contents.c_str());
						if (doc.HasParseError()) {
							CCLOG("GetParseError %s\n", doc.GetParseError());
							//signal error
							events->emit<Events::Error>(entity, "JSONReader system file read error");
							//WHAT TO DO ?
						}
						//else everything is ok with this json file
					}
					else // read fail : lets create first content
					{
						doc.SetObject();
						rapidjson::StringBuffer strbuf;
						rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
						doc.Accept(writer);
						file->m_contents = strbuf.GetString();
						//CCLOG("new contents :  %s", file->m_contents.c_str());
					}

					entity.remove<Comp::Read>();
				}
			/*

					if (!entity.component<Comp::JSONDoc>())
					{
						if (cocos2d::FileUtils::getInstance()->isFileExist(file->getFilename()))
						{
							FILE* fp = file->open("r+");
							//create a json doc for this file
							auto jdoc = entity.assign<Comp::JSONDoc>(fp);
							jdoc->getDoc().ParseStream<0>(jdoc->getStream());
							if (jdoc->getDoc().HasParseError()) {
								CCLOG("GetParseError %s\n", jdoc->getDoc().GetParseError());
								//signal error
								events->emit<Events::Error>(entity);
							}
							//to reset the file cursor at the beginning after read;
							fseek(fp, 0, 0);
						}
						else
						{
							FILE* fp = file->open("w+");
							//create a json doc for this file
							auto jdoc = entity.assign<Comp::JSONDoc>(fp);
							//file didnt exist. no need to read it.
							//define doc as object instead of array.
							jdoc->getDoc().SetObject();
						}

					}
				}
				*/
			}

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos


