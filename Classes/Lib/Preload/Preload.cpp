#include "WkCocos/Preload/Preload.h"

#include "WkCocos/Preload/Comp/DataLoad.h"

#include "WkCocos/Preload/Systems/DataEval.h"
#include "WkCocos/Preload/Systems/SyncLoading.h"
#include "WkCocos/Preload/Systems/ASyncLoading.h"
#include "WkCocos/Preload/Systems/ProgressUpdate.h"

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
			//, m_error_callback(error_callback)
		{
			//curl_global_init(CURL_GLOBAL_DEFAULT);
		}

		Preload::~Preload()
		{
			//curl_global_cleanup();
		}

		bool Preload::addDataLoad(const std::vector<std::string> &  filepath)
		{
			for (auto path : filepath)
			{
				entityx::Entity entity = entity_manager->create();
				entity.assign<Comp::DataLoad>(path);
				entity.assign<Comp::ProgressValue>(1);
			}
			return true;
		}

		void Preload::configure()
		{
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
		};

		void Preload::initialize()
		{
			//adding writable path ( where DLC downloads) as search path. First in list
			int i = 0;
			cocos2d::FileUtils *fileUtils = cocos2d::FileUtils::getInstance();
			std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
			searchPaths.insert(searchPaths.begin() + i++, cocos2d::FileUtils::getInstance()->getWritablePath());
			//add more if needed
			fileUtils->setSearchPaths(searchPaths);
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

		void Preload::setEventEmmiter(entityx::ptr<entityx::EventManager> event_emmiter)
		{
			event_emmiter->subscribe<Download::Events::Downloaded>(*this);
		}

		void Preload::receive(const Download::Events::Downloaded &dl)
		{
			entityx::ptr<Comp::DataLoad> data;
			for (auto entity : entity_manager->entities_with_components(data))
			{
				if (data->getFilepath() == dl.m_filepath)
				{
					data->loaded = false;
					entity.assign<Comp::ProgressValue>(1);
				}
			}
		}

	} // namespace Preload
}  // namespace WkCocos
