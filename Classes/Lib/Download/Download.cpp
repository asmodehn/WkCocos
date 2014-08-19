#include "WkCocos/Download/Download.h"

#include "WkCocos/Download/Systems/DLClisting.h"
#include "WkCocos/Download/Systems/DLCchecking.h"
#include "WkCocos/Download/Systems/MD5checking.h"
#include "WkCocos/Download/Systems/CurlDL.h"
#include "WkCocos/Download/Systems/DLvalidating.h"
#include "WkCocos/Download/Systems/ProgressUpdate.h"

//#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

//#include "WkCocos/Utils/ToolBox.h"

#include <functional>

namespace WkCocos
{
	namespace Download
	{

		Download::Download(unsigned short concurrent_downloads,
			std::function<void(float)> progress_callback
			)
			: m_concurrent_downloads(concurrent_downloads)
			, m_progress_callback(progress_callback)
		{
			curl_global_init(CURL_GLOBAL_DEFAULT);
		}

		Download::~Download()
		{
			curl_global_cleanup();
		}

		void Download::addDataDownload(const std::string json_manifest_filename)
		{
			cocos2d::Data manifest_data = cocos2d::FileUtils::getInstance()->getDataFromFile(json_manifest_filename);
				
			rapidjson::Document json;

			std::string manifestStr = cocos2d::FileUtils::getInstance()->getStringFromFile(json_manifest_filename);

			json.Parse<0>(manifestStr.c_str());
			if (json.HasParseError()) {
				CCLOG("GetParseError %s\n", json.GetParseError());
			}

			bool dlcEnable = cocostudio::DictionaryHelper::getInstance()->getBooleanValue_json(json, "dlcEnable", "error");
			std::string dlcUrl = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "dlcUrl", "error");
			std::string minAppVersion = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "minAppVersion", "error");
			std::string version = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(json, "version", "0");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			auto locahost = dlcUrl.find("localhost");
			if ( std::string::npos != locahost)
			{
				// If we can find "localhost". Replace it with emulator access to localhost.
				dlcUrl.replace(locahost, 9, "10.0.2.2");
			}
#endif
				
			CCLOG("dlcUrl : %s", dlcUrl.c_str());
			CCLOG("minAppVersion : %s", minAppVersion.c_str());
			CCLOG("version : %s",version.c_str());

			unsigned long lver = 0;
			try {
				 lver = ToolBox::stoul(version);
				CCLOG("Manifest version %lu for DLC at %s ", lver, dlcUrl.c_str());
			}
			catch (std::out_of_range oor)
			{
				lver = LONG_MAX;
			}

			if (dlcEnable)
			{
				entityx::Entity entity = entity_manager->create();
				entity.assign<Comp::DataListDownload>(dlcUrl, lver , minAppVersion);
				entity.assign<Comp::ProgressValue>(1);
			}
			else
			{
				CCLOG("Manifest IGNORED !");
			}

		}

		void Download::configure()
		{
			auto dlc_list = system_manager->add<Systems::DLClisting>();
			dlc_list->setConnectionTimeout(5);

			auto dlc_check = system_manager->add<Systems::DLCchecking>();
			dlc_check->setConnectionTimeout(5);

			system_manager->add<Systems::MD5checking>();
			system_manager->add<Systems::CurlDL>(m_concurrent_downloads);
			system_manager->add<Systems::DLvalidating>();
			system_manager->add<Systems::ProgressUpdate>(m_progress_callback);
		};

		void Download::initialize()
		{
			//adding writable path ( where DLC downloads) as search path. First in list
			int i = 0;
			cocos2d::FileUtils *fileUtils = cocos2d::FileUtils::getInstance();
			std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
			searchPaths.insert(searchPaths.begin() + i++, cocos2d::FileUtils::getInstance()->getWritablePath());
			//add more if needed
			fileUtils->setSearchPaths(searchPaths);
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
