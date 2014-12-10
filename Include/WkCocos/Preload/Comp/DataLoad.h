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
				DataLoad(std::string filepath) : m_filepath(filepath), force(false), loaded(false) {}

				inline std::string getFilepath() const
				{
					return m_filepath;
				}
				std::string m_filepath;

				bool loaded;
				bool force; // to force the loading ( need to be set manually when needed )
			};

			struct DataDepends : entityx::Component<DataDepends> {
				DataDepends(const std::vector<std::string>& depends_filepath) 
				: m_depends_filepath(depends_filepath)
				{
					m_depends_loaded.resize(m_depends_filepath.size());
					for (auto loaded = m_depends_loaded.begin(); loaded != m_depends_loaded.end(); ++loaded)
					{
						*loaded = false;
					}
				}

				inline const std::vector<std::string>& getDependsFilepath() const
				{
					return m_depends_filepath;
				}

				void fileLoaded(const std::string& filePath)
				{
					auto curLoaded = m_depends_loaded.begin();
					for (auto curFile = m_depends_filepath.begin(); curFile != m_depends_filepath.end(); ++curFile, ++curLoaded)
					{
						if (filePath == *curFile)
						{
							*curLoaded = true;
							break;
						}
					}
				}

				inline bool allLoaded()
				{
					for (auto dpd : m_depends_loaded)
					{
						if (!dpd) 
							return false;
					}
					return true;
				}

				std::vector<std::string> m_depends_filepath;
				std::vector<bool> m_depends_loaded;
			};

		}//namespace Comp
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_COMP_DATALOAD_H__
