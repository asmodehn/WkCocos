#ifndef __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__
#define __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Download
	{
		namespace Events
		{
			struct Downloaded : public entityx::Event<Downloaded>
			{
				Downloaded(std::string filepath)
				: m_filepath(filepath)
				{}

				std::string m_filepath;
			};

		}//namespace Event
	}//namespace Download
}//namespace WkCocos


#endif // __DFGAME_DOWNLOAD_EVENTS_DOWNLOADED_H__
