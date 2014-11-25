#include "WkCocos/Preload/Preload.h"

#include "WkCocos/Preload/Comp/DataLoad.h"

#include "WkCocos/Preload/Systems/DataEval.h"
#include "WkCocos/Preload/Systems/SyncLoading.h"
#include "WkCocos/Preload/Systems/ASyncLoading.h"
#include "WkCocos/Preload/Systems/ProgressUpdate.h"

#include "WkCocos/Utils/log/logstream.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "WkCocos/Utils/ToolBox.h"

#include <functional>

namespace WkCocos
{
	namespace Preload
	{

		Preload::Preload(unsigned short concurrent_loads,
			std::function<void(float)> progress_callback/*,
			std::function<void(std::string)> error_callback*/
			)
			: m_concurrent_loads(concurrent_loads)
			, m_progress_callback(progress_callback)
			, event_manager(entityx::EventManager::make())
			, entity_manager(entityx::EntityManager::make(event_manager))
			, system_manager(entityx::SystemManager::make(entity_manager, event_manager))
			//, m_error_callback(error_callback)
		{
			//curl_global_init(CURL_GLOBAL_DEFAULT);
			//system_manager->add<Systems::Error>(m_error_callback);
			system_manager->add<Systems::DataEval>();
			//system_manager->add<Systems::DLClisting>();
			//system_manager->add<Systems::DLCchecking>();
			//system_manager->add<Systems::MD5checking>();
			//system_manager->add<Systems::CurlMultiDL>(m_concurrent_downloads);
			//system_manager->add<Systems::CurlDL>(m_concurrent_downloads);
			//system_manager->add<Systems::DLvalidating>();
			system_manager->add<Systems::ASyncLoading>(m_concurrent_loads);
			system_manager->add<Systems::SyncLoading>();
			system_manager->add<Systems::ProgressUpdate>(m_progress_callback);

			system_manager->configure();

			event_manager->subscribe<Events::Loaded>(*this);
			//adding writable path ( where DLC downloads) as search path. First in list
			int i = 0;
			cocos2d::FileUtils *fileUtils = cocos2d::FileUtils::getInstance();
			std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
			searchPaths.insert(searchPaths.begin() + i++, cocos2d::FileUtils::getInstance()->getWritablePath());
			//add more if needed
			fileUtils->setSearchPaths(searchPaths);
		}

		Preload::~Preload()
		{
			//curl_global_cleanup();
		}

		bool Preload::addDataLoad(const std::string &  filepath, const std::vector<std::string> & depends_filepath)
		{
			entityx::ptr<Comp::DataLoad> data;
			bool exist = false;
			for (auto res : entity_manager->entities_with_components(data))
			{
				if (data->getFilepath() == filepath)
				{
					exist = true;
					break;
				}
			}
			if (!exist)
			{
				entityx::Entity entity = entity_manager->create();
				entity.assign<Comp::DataLoad>(filepath);
				entity.assign<Comp::DataDepends>(depends_filepath);
				entity.assign<Comp::ProgressValue>(1);
			}
			else
			{
				LOG_INFO << "Preload > Resource \"" << filepath << "\" already added!" << std::endl;
			}

			return true;
		}
		
		void Preload::update(double dt) {

			//check for error and report them if needed
			//system_manager->update<Systems::Error>(dt);
			//evaluate entities containing DataLoad components
			system_manager->update<Systems::DataEval>(dt);

			//asynchronously load data
			system_manager->update<Systems::ASyncLoading>(dt);
			//synchronously load data
			system_manager->update<Systems::SyncLoading>(dt);

			//display the progress
			system_manager->update<Systems::ProgressUpdate>(dt);
		}

		void Preload::setEventEmitter(entityx::ptr<entityx::EventManager> event_emitter)
		{
			event_emitter->subscribe<Download::Events::Downloaded>(*this);
		}

		void Preload::receive(const Download::Events::Downloaded &dl)
		{
			std::string dlfpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dl.m_filepath);
			//getting cocos filepath for downloaded file ( inverse of fullpathForFilename() )
			std::string shortpath = cocos2d::FileUtils::getInstance()->filenameForFullPath(dlfpath);

			entityx::ptr<Comp::DataLoad> data;
			for (auto entity : entity_manager->entities_with_components(data))
			{
				if (data->getFilepath() == shortpath)
				{
					CCLOG("downloaded event received for %s. Forcing reload it into cocos2d-x", shortpath.c_str());
					data->loaded = false;
					data->force = true;
					entity.assign<Comp::ProgressValue>(1);
				}
			}
		}

		void Preload::receive(const Events::Loaded &dl)
		{
			entityx::ptr<Comp::DataDepends> depends;
			for (auto entity : entity_manager->entities_with_components(depends))
			{
				depends->fileLoaded(dl.getLoadedPath());
			}
		}

	} // namespace Preload
}  // namespace WkCocos
