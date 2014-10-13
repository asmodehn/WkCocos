#include "WkCocos/LocalData/LocalDataManager.h"

#include "WkCocos/LocalData/Systems/JSONReader.h"
#include "WkCocos/LocalData/Systems/JSONLoginID.h"
#include "WkCocos/LocalData/Systems/JSONPlayerData.h"
#include "WkCocos/LocalData/Systems/JSONWriter.h"

#include "WkCocos/LocalData/Comp/LocalData.h"

namespace WkCocos
{
	namespace LocalData
	{

		LocalDataManager::LocalDataManager(/*std::function<void(std::string)> error_CB*/)
		:/* m_error_callback(error_CB)
		,*/ event_manager(entityx::EventManager::make())
		, entity_manager(entityx::EntityManager::make(event_manager))
		, system_manager(entityx::SystemManager::make(entity_manager, event_manager))
		{

			//system_manager->add<Systems::Error>(m_error_callback);
			system_manager->add<Systems::JSONReader>();
			system_manager->add<Systems::JSONLoginID>();
			system_manager->add<Systems::JSONPlayerData>();
			system_manager->add<Systems::JSONWriter>();
			system_manager->configure();
		}

		LocalDataManager::~LocalDataManager()
		{
		}

		bool LocalDataManager::saveLoginID(std::string user, std::string passwd, std::string key, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "ID", key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::LoginID_v1>(user, passwd);
			}
			return true;
		}

		bool LocalDataManager::loadLoginID(std::function<void(std::string user, std::string passwd)> load_cb, std::string key, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "ID", key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::LoginID_v1>([=](std::string user, std::string passwd){
					//doing this in cocos thread to not mess up the current update pass
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
						load_cb(user, passwd);
					});
				});
			}
			return true;
		}
		
		bool LocalDataManager::saveData(const std::string& saveName, std::map<std::string, std::string> data, std::string key, short version)
		{
			auto systementity = entity_manager->create();
			systementity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "system", key);
			systementity.assign<Comp::Read>();
			if (1 == version)
			{
				systementity.assign<Comp::PlayerData_v1>(data["system"]);
			}

			auto searchentity = entity_manager->create();
			searchentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "search", key);
			searchentity.assign<Comp::Read>();
			if (1 == version)
			{
				searchentity.assign<Comp::PlayerData_v1>(data["search"]);
			}

			return true;
		}

		bool LocalDataManager::loadData(const std::string& saveName, std::function<void(std::map<std::string, std::string>)> load_cb, std::string key, short version)
		{
			auto systementity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			systementity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "system", key);
			systementity.assign<Comp::Read>();
			if (1 == version)
			{
				systementity.assign<Comp::PlayerData_v1>([=](std::string systemdata){
					//doing this in cocos thread to not mess up the current update pass
					auto searchentity = entity_manager->create();
					searchentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "search", key);
					searchentity.assign<Comp::Read>();
					if (1 == version)
					{
						searchentity.assign<Comp::PlayerData_v1>([=](std::string searchdata)
						{
							std::map<std::string, std::string> data;
							data["system"] = systemdata;
							data["search"] = searchdata;
							cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
							{
								load_cb(data);
							});
						});
					}
				});
			}
			return true;
		}

		bool LocalDataManager::deleteData(const std::string& saveName, std::function<void(std::string data)> delete_cb, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + saveName, "", true); //we dont care about the key here
			newentity.assign<Comp::Read>();
			return true;
		}

		void LocalDataManager::update(double dt) {

			//check for error and report them if needed
			//system_manager->update<Systems::Error>(dt);
			system_manager->update<Systems::JSONReader>(dt);
			system_manager->update<Systems::JSONLoginID>(dt);
			system_manager->update<Systems::JSONPlayerData>(dt);
			system_manager->update<Systems::JSONWriter>(dt);

		}




	} // namespace LocalData
}  // namespace WkCocos
