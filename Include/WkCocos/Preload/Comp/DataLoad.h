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

			struct DataDepends : entityx::Component<DataDepends> {
				DataDepends(const std::vector<std::string>& depends_filepath) : m_depends_filepath(depends_filepath), depends_loaded(false) {}

				inline const std::vector<std::string>& getDependsFilepath() const
				{
					return m_depends_filepath;
				}

				inline bool allLoaded()
				{
					for ( auto dpd : depends_loaded )
					{
						if (!dpd) return false; 
						break;
					}
					return true;
				}

				std::vector<std::string> m_depends_filepath;
				std::vector<bool> depends_loaded;
			};

		}//namespace Comp
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_COMP_DATALOAD_H__
