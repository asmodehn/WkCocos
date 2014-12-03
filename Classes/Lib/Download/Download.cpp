#include "WkCocos/Download/Download.h"

#include "WkCocos/Download/Systems/DLClisting.h"
#include "WkCocos/Download/Systems/DLCchecking.h"
#include "WkCocos/Download/Systems/MD5checking.h"
#include "WkCocos/Download/Systems/CurlDL.h"
#include "WkCocos/Download/Systems/DLvalidating.h"
#include "WkCocos/Download/Systems/ProgressUpdate.h"

#include "WkCocos/Utils/jni/Utils.h"

//#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

//#include "WkCocos/Utils/ToolBox.h"

#include <functional>

namespace WkCocos
{
	namespace Download
	{

		Download::Download(unsigned short concurrent_downloads,
			std::function<void(float)> progress_callback //TODO : remove this
			)
			: event_manager(entityx::EventManager::make())
            , entity_manager(entityx::EntityManager::make(event_manager))
            , system_manager(entityx::SystemManager::make(entity_manager, event_manager))
            , m_concurrent_downloads(concurrent_downloads)
			, m_progress_callback(progress_callback)
		{
			curl_global_init(CURL_GLOBAL_DEFAULT);

			auto dlc_list = system_manager->add<Systems::DLClisting>();
			dlc_list->setConnectionTimeout(5);

			auto dlc_check = system_manager->add<Systems::DLCchecking>();
			dlc_check->setConnectionTimeout(5);

			system_manager->add<Systems::MD5checking>();
			system_manager->add<Systems::CurlDL>(m_concurrent_downloads);
			system_manager->add<Systems::DLvalidating>();
			system_manager->add<Systems::ProgressUpdate>(m_progress_callback);
            system_manager->configure();

            //adding writable path ( where DLC downloads) as search path. First in list
			int i = 0;
			cocos2d::FileUtils *fileUtils = cocos2d::FileUtils::getInstance();
			std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
			searchPaths.insert(searchPaths.begin() + i++, cocos2d::FileUtils::getInstance()->getWritablePath());
			//add more if needed
			fileUtils->setSearchPaths(searchPaths);
		}

		Download::~Download()
		{
			curl_global_cleanup();
		}

		void Download::addDataDownload(Version currentVersion, const std::string json_manifest_filename)
		{
			rapidjson::Document json;

			std::string manifestStr = cocos2d::FileUtils::getInstance()->getStringFromFile(json_manifest_filename);

			json.Parse<0>(manifestStr.c_str());
			if (json.HasParseError()) {
				CCLOG("GetParseError %s\n", json.GetParseError());
			}

			bool dlcEnable = cocostudio::DictionaryHelper::getInstance()->getBooleanValue_json(json, "dlcEnable", false);
			std::string dlcUrl = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "dlcUrl", "error");
			std::string dataVersion = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "dataVersion", "0");

//hack to override localhost DLC URL when we cross build for android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			auto locahost = dlcUrl.find("localhost");
			if ( std::string::npos != locahost)
			{
				// If we can find "localhost". Replace it with emulator access to localhost.
				dlcUrl.replace(locahost, 9, "10.0.2.2");
			}
#endif

			CCLOG("dlcUrl : %s", dlcUrl.c_str());
			CCLOG("dataVersion : %s", dataVersion.c_str());
			CCLOG("dlcEnable : %s", dlcEnable ? "true" : "false");

            Version dver(dataVersion);

			if (dlcEnable)
			{
				entityx::Entity entity = entity_manager->create();
				entity.assign<Comp::DataListDownload>(dlcUrl, dver , currentVersion);
				entity.assign<Comp::ProgressValue>(1);
			}
			else
			{
				CCLOG("Manifest IGNORED !");
			}

		}

		std::pair<int,int> Download::getCurrentProgress()
		{
            int downCurProgVal = system_manager->system<WkCocos::Download::Systems::ProgressUpdate>()->curProgVal;
            int downTotProgVal = system_manager->system<WkCocos::Download::Systems::ProgressUpdate>()->totalProgValMax;

            return std::make_pair(downCurProgVal,downTotProgVal);
		}

		void Download::update(double dt)
		{
			//listing versions avialable on DLC
			system_manager->update<Systems::DLClisting>(dt);
			//listing files in one version on DLC
			system_manager->update<Systems::DLCchecking>(dt);
			//check MD5 of files existing and downloaded
			system_manager->update<Systems::MD5checking>(dt);

			//do the curl calls when needed
			system_manager->update<Systems::CurlDL>(dt);

			//validates if signature matches
			system_manager->update<Systems::DLvalidating>(dt);

			//display the progress
			system_manager->update<Systems::ProgressUpdate>(dt);

		}

	} // namespace Download
}  // namespace WkCocos
