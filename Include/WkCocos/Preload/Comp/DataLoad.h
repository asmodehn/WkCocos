#ifndef __DFGAME_PRELOAD_ENTITY_COMP_DATALOAD_H__
#define __DFGAME_PRELOAD_ENTITY_COMP_DATALOAD_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Comp
		{
			struct DataLoad : entityx::Component<DataLoad> {
				DataLoad(std::string filepath) : m_filepath(filepath), loaded(false) {}

				inline std::string getFilepath() const
				{
					return m_filepath;
				}
				std::string m_filepath;

				bool loaded;
			};

		}//namespace Comp
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_COMP_DATALOAD_H__
