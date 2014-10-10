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
					login(userid, password, success_callback);
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

		void OnlineDataManager::save(const std::string& userid, const std::string& saveName, std::map<std::string, std::string> user_data, std::function<void(std::string)> success_callback)
		{
			//this code is not good! app42 can think we spam it with requests!
			if (system != "")
			{
				auto systemupdateentity = entity_manager->create();
				systemupdateentity.assign < Comp::UpdateUserData >(userid, saveName, system, user_data["system"], [=](::App42::App42StorageResponse* r)
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
							success_callback(r->getBody());
						});
					}
				});
			}
			else
			{
				auto systeminsertentity = entity_manager->create();
				systeminsertentity.assign < Comp::InsertUserData >(userid, saveName, user_data["system"], [=](::App42::App42StorageResponse* r)
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
							success_callback(r->getBody());
						});
					}
				});
			}

			if (search != "")
			{
				auto searchupdateentity = entity_manager->create();
				searchupdateentity.assign < Comp::UpdateUserData >(userid, saveName, search, user_data["search"], [=](::App42::App42StorageResponse* r)
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
							success_callback(r->getBody());
						});
					}
				});
			}
			else
			{
				auto searchinsertentity = entity_manager->create();
				searchinsertentity.assign < Comp::InsertUserData >(userid, saveName, user_data["search"], [=](::App42::App42StorageResponse* r)
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
							success_callback(r->getBody());
						});
					}
				});
			}
		}

		void OnlineDataManager::load(const std::string& userid, const std::string& saveName, std::function<void(std::map<std::string, std::string>)> callback)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::LoadUserData>(userid, saveName, [=](::App42::App42UserResponse* r)
			{
				if (r->isSuccess)
				{
					std::map<std::string, std::string> docs;
					std::vector<::App42::JSONDocument> jsonDocArray = r->users.front().jsonDocArray;
					for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
					{
						rapidjson::Document doc;
						rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
						auto jsonDoc = it->getJsonDoc();
						doc.Parse<0>(jsonDoc.c_str());
						if (doc.HasMember("system"))
						{
							system = it->getDocId();
							docs["system"] = jsonDoc;
						}
						if (doc.HasMember("search"))
						{
							search = it->getDocId();
							docs["search"] = jsonDoc;
						}
					}
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, callback, docs]()
					{
						callback(docs);
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

		void OnlineDataManager::getUsersWithDocs(const std::string& saveName)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersWithDocs>(saveName, [=](std::string data)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data](){
					event_manager->emit<Events::PlayersList>(data);
				});
			});
		}

		void OnlineDataManager::getUsersKeyValue(const std::string& saveName, const std::string& key, int value, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersKeyValue>(saveName, key, value, quantity, offset, [=](std::string data)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data](){
					event_manager->emit<Events::PlayersList>(data);
				});
			});
		}

		void OnlineDataManager::getUsersFromTo(const std::string& saveName, const std::string& key, int from, int to, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersFromTo>(saveName, key, from, to, quantity, offset, [=](std::string data)
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, data](){
					event_manager->emit<Events::PlayersList>(data);
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

		void OnlineDataManager::update(double dt) {
			//check for error and report them if needed
			system_manager->update<Systems::Storage>(dt);
			system_manager->update<Systems::User>(dt);
			system_manager->update<Systems::Timer>(dt);
		}

	} // namespace LocalData
}  // namespace WkCocos
