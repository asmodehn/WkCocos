#include "WkCocos/OnlineData/OnlineDataManager.h"

#include "WkCocos/OnlineData/Systems/User.h"
#include "WkCocos/OnlineData/Systems/Storage.h"
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
			system_manager->configure();
		}
		
		OnlineDataManager::~OnlineDataManager()
		{
		}

		void OnlineDataManager::loginNew(std::string userid, std::string password, std::string email, std::function<void(std::string)> success_callback)
		{

			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
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
			//new File component for each request. The aggregator system will detect duplicates and group them
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

		void OnlineDataManager::save(std::string userid, std::string user_data, std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::DeleteUserData>(userid, "user_data", user_data, callback);
			newentity.assign<Comp::SaveUserData>(userid, "user_data", user_data, callback);
		}

		void OnlineDataManager::load(std::string userid, std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::LoadUserData>(userid, "user_data", callback);
		}

		void OnlineDataManager::getAllUsers()
		{
			auto newentity = entity_manager->create();
			//new File component for each request. The aggregator system will detect duplicates and group them
			newentity.assign<Comp::GetAllUsers>(event_manager);
		}

		void OnlineDataManager::update(double dt) {
			//check for error and report them if needed
			system_manager->update<Systems::Storage>(dt);
			system_manager->update<Systems::User>(dt);
		}

	} // namespace LocalData
}  // namespace WkCocos
