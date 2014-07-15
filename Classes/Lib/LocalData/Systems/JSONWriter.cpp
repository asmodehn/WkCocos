#include "WkCocos/LocalData/Systems/JSONWriter.h"

#include "WkCocos/LocalData/Events/Error.h"
#include "WkCocos/LocalData/Comp/LocalData.h"

//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/prettywriter.h"     // for stringify JSON
#include "json/filestream.h"       // wrapper of C stream for prettywriter as output
#include "json/stringbuffer.h"

namespace WkCocos
{
	namespace LocalData
	{
		namespace Systems
		{
			/**
			* This function will write into json file
			*/
			void JSONWriter::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::File> file;
				for (auto entity : es->entities_with_components(file))
				{
					//if we have only a file component : all updates have been done, we can write the file and finish this
					if (entity.component_mask() == ( uint64_t(1) << Comp::File::family()))
					{
						bool filewritten = file->write();
						if (filewritten)
						{
							//we wrote all changes we can destroy this
							entity.destroy();
						}
						else
						{
							CCLOG("file write error !!\n",);
							//signal error
							events->emit<Events::Error>(entity);
							//WHAT TO DO ?
						}
					}
					//else there are more updates to do on next update
				}

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos


