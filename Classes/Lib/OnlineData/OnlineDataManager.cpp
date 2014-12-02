#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/OnlineData/Systems/User.h"
#include "WkCocos/OnlineData/Systems/Storage.h"
#include "WkCocos/OnlineData/Systems/Timer.h"
#include "WkCocos/OnlineData/Systems/ProgressUpdate.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "cocos/cocos2d.h"
//#include "Common/App42API.h"

#include "WkCocos/OnlineData/Events/Error.h"

#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

#include <sstream>

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
			system_manager->add<Systems::ProgressUpdate>();
			system_manager->configure();
		}

		OnlineDataManager::~OnlineDataManager()
		{
		}

		void OnlineDataManager::loginNew(std::string userid, std::string password, std::string email, std::function<void(std::string)> success_callback)
		{

			auto newentity = entity_manager->create();
			auto id = newentity.id();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::Create>(userid, password, email, [=](RequestStatus rs){
                //error handling done on next cocos update
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,rs,userid,password,success_callback](){
                    if ( rs.getStatus() == Status::REQUEST_TIMEOUT || rs.getStatus() == Status::REQUEST_ERROR )
                    {
                        this->event_manager->emit<Events::Error>(rs);
                    }
                    else if( rs.getStatus() == Status::REQUEST_SUCCESS)
                    {
                        //potentially avoiding race condition by doing login from cocos thread as usual.
                        login(userid, password, success_callback);
                    }
                });
            });
		}

		void OnlineDataManager::login(std::string userid, std::string password, std::function<void(std::string)> success_callback)
		{
			auto newentity = entity_manager->create();
			auto id = newentity.id();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::Login>(userid, password, [=](RequestStatus rs, std::string body){
                //error handling done on next cocos update
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,rs,body,success_callback](){
                    if ( rs.getStatus() == Status::REQUEST_TIMEOUT || rs.getStatus() == Status::REQUEST_ERROR )
                    {
                        this->event_manager->emit<Events::Error>(rs);
                    }
                    else if( rs.getStatus() == Status::REQUEST_SUCCESS)
                    {
                        success_callback(body);
                    }
                });
            });
		}

		entityx::Entity::Id OnlineDataManager::save(const std::string& userid, const std::string& saveName, std::string docId, std::string user_data, std::function<void(std::string, std::string, std::string)> success_callback, std::string key)
		{
			auto updateentity = entity_manager->create();
			auto id = updateentity.id();
			updateentity.assign<Comp::ProgressUpdate>();
			updateentity.assign < Comp::UpdateUserData >(userid, saveName, docId, user_data, success_callback);
			return id;
		}

		entityx::Entity::Id OnlineDataManager::saveNew(const std::string& userid, const std::string& saveName, std::string user_data, std::function<void(std::string, std::string, std::string)> success_callback, std::string key)
		{
			auto insertentity = entity_manager->create();
			auto id = insertentity.id();
			insertentity.assign<Comp::ProgressUpdate>();
			insertentity.assign < Comp::InsertUserData >(userid, saveName, user_data, success_callback);
			return id;
		}

		entityx::Entity::Id OnlineDataManager::load(const std::string& userid, const std::string& saveName, std::function<void(std::string, std::vector<std::string>)> callback, std::string key)
		{
			auto newentity = entity_manager->create();
			auto id = newentity.id();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::LoadUserData>(userid, saveName, [=](RequestStatus rs, std::string docId, std::vector<std::string> docList){
                //error handling done on next cocos update
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,rs,docId,docList,callback](){
                    if ( rs.getStatus() == Status::REQUEST_TIMEOUT || rs.getStatus() == Status::REQUEST_ERROR )
                    {
                        this->event_manager->emit<Events::Error>(rs);
                    }
                    else if( rs.getStatus() == Status::REQUEST_SUCCESS )
                    {
                        callback(docId,docList);
                    }
                });
            });
			return id;
		}

		void OnlineDataManager::getUsersKeyValue(const std::string& saveName, const std::string& key, int value, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::GetUsersKeyValue>(saveName, key, value, quantity, offset);
		}

		void OnlineDataManager::getUsersFromTo(const std::string& saveName, const std::string& key, int from, int to, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::GetUsersFromTo>(saveName, key, from, to, quantity, offset);
		}

		void OnlineDataManager::getServerTime(std::function<void(std::string)> callback)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::ServerTime>(callback);
		}

		void OnlineDataManager::getAllDocsPaging(const std::string& saveName, int quantity, int offset)
		{
			auto newentity = entity_manager->create();
			newentity.assign<Comp::ProgressUpdate>();
			newentity.assign<Comp::AllDocsPaging>(saveName, quantity, offset);
		}

		void OnlineDataManager::update(double dt) {
			//check for error and report them if needed
			system_manager->update<Systems::Storage>(dt);
			system_manager->update<Systems::User>(dt);
			system_manager->update<Systems::Timer>(dt);
			system_manager->update<Systems::ProgressUpdate>(dt);
		}

	} // namespace LocalData
}  // namespace WkCocos
