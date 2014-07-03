#ifndef __DFGAME_SLIDESHOW_EVENTS_LOADED_H__
#define __DFGAME_SLIDESHOW_EVENTS_LOADED_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Events
		{
			struct Loaded : public entityx::Event<Loaded>
			{
				Loaded(std::string rpath) : loaded_path(rpath)
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
}//namespace dfgame


#endif // __DFGAME_SLIDESHOW_EVENTS_ERROR_H__
