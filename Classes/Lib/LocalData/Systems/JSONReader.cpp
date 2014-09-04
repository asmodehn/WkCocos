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
					if (fileread && file->getContents() != "")
					{
						doc.Parse<0>(file->getContents().c_str());
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
						file->setContents( strbuf.GetString());
						//CCLOG("new contents :  %s", file->getContents().c_str());
					}

					entity.remove<Comp::Read>();
				}
			}

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos


