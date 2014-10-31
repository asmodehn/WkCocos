#include "WkCocos/OnlineData/OnlineDataManager.h"

#include "WkCocos/OnlineData/Systems/User.h"
#include "WkCocos/OnlineData/Systems/Storage.h"
#include "WkCocos/OnlineData/Systems/Timer.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "cocos/cocos2d.h"
//#include "Common/App42API.h"

#include "WkCocos/OnlineData/Events/Error.h"

#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace WkCocos
{
	namespace OnlineData
	{
		OnlineDataManager::OnlineDataManager(std::string app_access_key, std::string app_secret_key)
			: event_manager(entityx::EventManager::make())
			, entity_manager(entityx::EntityManager::make(event_manager))
			, system_manager(entityx::SystemManager::make(entity_manager, event_manager))
		{
			::App42::App42API::Initialize(app_access_key, app_secret_key);
#ifdef _DEBUG
			::App42::App42API::setIsTraceEnabled(true);
#endif
			system_manager->add<Systems::User>();
			system_manager->add<Systems::Storage>();
			system_manager->add<Systems::Timer>();
			system_manager->configure();
		}
		
		OnlineDataManager::~OnlineDataManager()
		{
		}

		void OnlineDataManager::loginNew(std::string userid, std::string password, std::string email, std::function<void(std::string)> success_callback)
		{

			auto newentity = entity_manager->create();
			newentity.assign<Comp::Create>(userid, password, email, [=](::App42::App42UserResponse* r){

				if (r->isSuccess)
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, userid, password, success_callback](){
						login(userid, password, success_callback);
					});
				}
				else // if creation failed, emit event ( in cocos thread to allow cocos actions )
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, r](){
						event_manager->emit<Events::Error>(r);
					});
				}
				
			});
			
		}

		void OnlineDataManager::login(std::string userid, std::string password, std::function<void(std::string)> success_callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::Login>(userid, password, [=](::App42::App42UserResponse* r){
				if (! r->isSuccess)
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, r](){
						event_manager->emit<Events::Error>(r);
						//callback is not called if error
					});
				}
				else
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, success_callback, r](){
						success_callback(r->getBody());
					});
				}
			});
		}

		void OnlineDataManager::save(const std::string& userid, const std::string& saveName, std::string docId, std::string user_data, std::function<void(std::string, std::string, std::string)> success_callback, std::string key)
		{
			auto updateentity = entity_manager->create();
			updateentity.assign < Comp::UpdateUserData >(userid, saveName, docId, user_data, [=](::App42::App42StorageResponse* r)
			{
				if (!r->isSuccess)
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, r](){
						event_manager->emit<Events::Error>(r);
						//callback is not called if error
					});
				}
				else
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, success_callback, r](){
						success_callback(r->storages.back().collectionName, r->storages.back().jsonDocArray.back().getDocId(), r->getBody());
					});
				}
			});
		}

		void OnlineDataManager::saveNew(const std::string& userid, const std::string& saveName, std::string user_data, std::function<void(std::string, std::string, std::string)> success_callback, std::string key)
		{
			auto insertentity = entity_manager->create();
			insertentity.assign < Comp::InsertUserData >(userid, saveName, user_data, [=](::App42::App42StorageResponse* r)
			{
				if (!r->isSuccess)
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, r](){
						event_manager->emit<Events::Error>(r);
						//callback is not called if error
					});
				}
				else
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, success_callback, r](){
						success_callback(r->storages.back().collectionName, r->storages.back().jsonDocArray.back().getDocId(), r->getBody());
					});
				}
			});
		}
		
		void OnlineDataManager::load(const std::string& userid, const std::string& saveName, std::function<void(std::string, std::vector<std::string>)> callback, std::string key)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::LoadUserData>(userid, saveName, [=](::App42::App42UserResponse* r)
			{
				if (r->isSuccess)
				{
					std::vector<std::string> docs;
					std::vector<::App42::JSONDocument> jsonDocArray = r->users.front().jsonDocArray;
					std::string docId  ="";

					if (!jsonDocArray.empty())
					{
						//id of last doc is the one we want to use ( others are discarded )
						docId = jsonDocArray.back().getDocId();

						for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
						{
							rapidjson::Document doc;
							rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
							auto jsonDoc = it->getJsonDoc();
							docs.push_back(jsonDoc.c_str());
						}
					}

					//callback called even if data not present
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, callback, docId, docs]()
					{
						callback(docId, docs);
					});

				}
				else// if request failed, 
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, r](){
						event_manager->emit<Events::Error>(r);
						//callback is not called if error
					});
				}
			});
		}

		void OnlineDataManager::getUsersKeyValue(const std::string& saveName, const std::string& key, int value, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersKeyValue>(saveName, key, value, quantity, offset, [=](std::map<std::string, std::string> data, int recordCount)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data, recordCount](){
					event_manager->emit<Events::PlayersList>(data, recordCount);
				});
			});
		}

		void OnlineDataManager::getUsersFromTo(const std::string& saveName, const std::string& key, int from, int to, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersFromTo>(saveName, key, from, to, quantity, offset, [=](std::map<std::string, std::string> data, int recordCount)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data, recordCount](){
					event_manager->emit<Events::PlayersList>(data, recordCount);
				});
			});
		}

		void OnlineDataManager::getServerTime(std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ServerTime>([=](std::string s_iso8601){
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, callback, s_iso8601](){
					callback(s_iso8601);
				});
			});
		}

		void OnlineDataManager::getAllDocsPaging(const std::string& saveName, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::AllDocsPaging>(saveName, quantity, offset, [=](std::vector<std::map<std::string, std::string>> data)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data](){
					event_manager->emit<Events::DocsList>(data);
				});
			});
		}

		void OnlineDataManager::update(double dt) {
			//check for error and report them if needed
			system_manager->update<Systems::Storage>(dt);
			system_manager->update<Systems::User>(dt);
			system_manager->update<Systems::Timer>(dt);
		}

	} // namespace LocalData
}  // namespace WkCocos
