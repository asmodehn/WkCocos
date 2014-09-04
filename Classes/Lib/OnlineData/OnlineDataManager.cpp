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
				else // if creation failed, 
				{
					event_manager->emit<Events::Error>(r);
				}
				
			});
			
		}

		void OnlineDataManager::login(std::string userid, std::string password, std::function<void(std::string)> success_callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::Login>(userid, password, [=](::App42::App42UserResponse* r){
				if (! r->isSuccess)
				{
					event_manager->emit<Events::Error>( r );
					//callback is not called if error
				}
				else
				{
					success_callback(r->getBody());
				}
			});
		}

		void OnlineDataManager::save(std::string userid, std::string user_data, std::function<void(std::string)> success_callback)
		{
			auto newentity = entity_manager->create();
			//temp comment
			//newentity.assign<Comp::DeleteUserData>(userid, "user_data", user_data, callback);
			newentity.assign<Comp::SaveUserData>(userid, m_collection, user_data, [=](::App42::App42UserResponse* r){
				if (!r->isSuccess)
				{
					event_manager->emit<Events::Error>(r);
					//callback is not called if error
				}
				else
				{
					success_callback(r->getBody());
				}
			});
		}

		void OnlineDataManager::load(std::string userid, std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::LoadUserData>(userid, m_collection, callback);
		}

		void OnlineDataManager::loadEnemy(std::string userid)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::LoadEnemyData>(userid, m_collection, event_manager);
		}

		void OnlineDataManager::getAllUsers()
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetAllUsers>(event_manager);
		}

		void OnlineDataManager::getUsersWithDocs()
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::GetUsersWithDocs>(event_manager);
		}

		void OnlineDataManager::getServerTime()
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ServerTime>(event_manager);
		}

		void OnlineDataManager::update(double dt) {
			//check for error and report them if needed
			system_manager->update<Systems::Storage>(dt);
			system_manager->update<Systems::User>(dt);
			system_manager->update<Systems::Timer>(dt);
		}

	} // namespace LocalData
}  // namespace WkCocos
