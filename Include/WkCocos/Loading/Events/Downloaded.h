#ifndef __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__
#define __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Events
		{
			struct Downloaded : public entityx::Event<Downloaded>
			{
				Downloaded(std::string rpath) : loaded_path(rpath)
				{}

				inline std::string getLoadedPath() const
				{
					return loaded_path;
				}

			private:
				std::string loaded_path;
			};

		}//namespace Event
	}//namespace Loading
}//namespace WkCocos


#endif // __DFGAME_SLIDESHOW_EVENTS_DOWNLOADED_H__
