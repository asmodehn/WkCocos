#include "WkCocos/OnlineData/OnlineDataManager.h"

#include "WkCocos/OnlineData/Systems/User.h"
#include "WkCocos/OnlineData/Systems/Storage.h"
#include "WkCocos/OnlineData/Systems/Timer.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "cocos/cocos2d.h"
//#include "Common/App42API.h"

#include "WkCocos/OnlineData/Events/Error.h"

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

		void OnlineDataManager::save(const std::string& userid, const std::string& saveName, std::string user_data, std::function<void(std::string)> success_callback)
		{
			auto newentity = entity_manager->create();
			//temp comment
			newentity.assign<Comp::FindUserData>(userid, saveName, [=](std::string doc)
			{
				auto delentity = entity_manager->create();
				delentity.assign < Comp::DeleteUserData >(userid, saveName, doc);
			}, [=]()
			{
				auto saveentity = entity_manager->create();
				saveentity.assign < Comp::SaveUserData >(userid, saveName, user_data, [=](::App42::App42UserResponse* r)
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
			});
		}

		void OnlineDataManager::load(const std::string& userid, const std::string& saveName, std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::LoadUserData>(userid, saveName, callback);
		}

		void OnlineDataManager::getUsersWithDocs(const std::string& saveName)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersWithDocs>(saveName, event_manager);
		}

		void OnlineDataManager::getUsersKeyValue(const std::string& saveName, const std::string& key, int value)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersKeyValue>(saveName, key, value, event_manager);
		}

		void OnlineDataManager::getUsersFromTo(const std::string& saveName, const std::string& key, int from, int to)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersFromTo>(saveName, key, from, to, event_manager);
		}

		void OnlineDataManager::getServerTime(std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ServerTime>([=](std::string s_iso8601){
				callback(s_iso8601);
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
