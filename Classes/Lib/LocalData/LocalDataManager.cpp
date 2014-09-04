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
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "LocalID.txt", key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::LoginID_v1>(user,passwd);
			}
			return true;
		}

		bool LocalDataManager::loadLoginID(std::function<void(std::string user, std::string passwd)> load_cb, std::string key, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + "LocalID.txt", key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::LoginID_v1>(load_cb);
			}
			return true;
		}
		
		bool LocalDataManager::saveData(const std::string& saveName, std::string data, std::string key, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + saveName, key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::PlayerData_v1>(data);
			}
			return true;
		}

		bool LocalDataManager::loadData(const std::string& saveName, std::function<void(std::string data)> load_cb, std::string key, short version)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::File>(cocos2d::FileUtils::getInstance()->getWritablePath() + saveName, key);
			newentity.assign<Comp::Read>();
			if (1 == version)
			{
				newentity.assign<Comp::PlayerData_v1>(load_cb);
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
