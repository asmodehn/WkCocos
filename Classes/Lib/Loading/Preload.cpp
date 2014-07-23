#include "WkCocos/Loading/Preload.h"

#include "WkCocos/Loading/Systems/Error.h"
#include "WkCocos/Loading/Systems/DataEval.h"
//#include "WkCocos/Loading/Systems/DLClisting.h"
//#include "WkCocos/Loading/Systems/DLCchecking.h"
//#include "WkCocos/Loading/Systems/MD5checking.h"
//#include "WkCocos/Loading/Systems/CurlDL.h"
//#include "WkCocos/Loading/Systems/DLvalidating.h"
#include "WkCocos/Loading/Systems/Loading.h"
#include "WkCocos/Loading/Systems/ProgressUpdate.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "WkCocos/Utils/ToolBox.h"

#include <functional>

namespace WkCocos
{
	namespace Loading
	{

		Preload::Preload(unsigned short concurrent_loads,
			std::function<void(float)> progress_callback,
			std::function<void()> error_callback
			)
			: m_concurrent_loads(concurrent_loads)
			, m_progress_callback(progress_callback)
			, m_error_callback(error_callback)
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
			system_manager->add<Systems::Error>(m_error_callback);
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
			system_manager->update<Systems::Error>(dt);
			//evaluate entities containing DataLoad components
			system_manager->update<Systems::DataEval>(dt);
			//listing versions avialable on DLC
			//system_manager->update<Systems::DLClisting>(dt);
			//listing files in one version on DLC
			//system_manager->update<Systems::DLCchecking>(dt);
			//check MD5 of files existing and downloaded
			//system_manager->update<Systems::MD5checking>(dt);
			
			//do the curl calls when needed
			//system_manager->update<Systems::CurlMultiDL>(dt);
			//system_manager->update<Systems::CurlDL>(dt);

			//validates if signature matches
			//system_manager->update<Systems::DLvalidating>(dt);
			//asynchronously load data
			system_manager->update<Systems::ASyncLoading>(dt);
			//synchronously load data
			system_manager->update<Systems::SyncLoading>(dt);

			//display the progress
			system_manager->update<Systems::ProgressUpdate>(dt);
		}

		void Preload::setEventEmmiter(entityx::ptr<entityx::EventManager> event_emmiter)
		{
			event_emmiter->subscribe<Events::Downloaded>(*system_manager->system<Systems::DataEval>());
		}

	} // namespace Loading
}  // namespace WkCocos
