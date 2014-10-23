#ifndef __WKCOCOS_ONLINEDATA_ONLINEDATAMANAGER_H__
#define __WKCOCOS_ONLINEDATA_ONLINEDATAMANAGER_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Events/PlayersList.h"
#include "WkCocos/OnlineData/Events/DocsList.h"

#define DEFAULT_SAVE_VERSION 1

namespace WkCocos
{
	namespace OnlineData
	{
		class OnlineDataManager
		{
		public:
			/**
			* Constructor 
			*/
			OnlineDataManager(std::string app_access_key, std::string app_secret_key);

			/**
			* Destructor
			*/
			~OnlineDataManager();
			
			/**
			* Create User
			*/
			void loginNew(std::string userid, std::string password, std::string email, std::function<void(std::string)> callback);
			
			/**
			* Login User
			*/
			void login(std::string userid, std::string password, std::function<void(std::string)> callback);

			/**
			* Save User Data
			*/
			void save(const std::string& userid, const std::string& saveName, std::string docId, std::string data, std::function<void(std::string)> callback, std::string key = "");

			/**
			* Save New User Data
			*/
			void saveNew(const std::string& userid, const std::string& saveName, std::string data, std::function<void(std::string)> success_callback, std::string key = "");

			/**
			* Load User Data
			*/
			void load(const std::string& userid, const std::string& saveName, std::function<void(std::string,std::vector<std::string>)> callback, std::string key = "");

			/**
			* Get Selected Users
			*/
			void getUsersKeyValue(const std::string& saveName, const std::string& key, int value, int quantity, int offset);
			void getUsersFromTo(const std::string& saveName, const std::string& key, int from, int to, int quantity, int offset);

			/**
			* Get Server Time
			*/
			void getServerTime(std::function<void(std::string)> callback);

			void getAllDocsPaging(const std::string& saveName, int quantity, int offset);

			void update(double dt);

			//subscribe to this event manager to receive error events.
			entityx::ptr<entityx::EventManager> getEventManager()
			{
				return event_manager;
			}

			entityx::ptr<entityx::SystemManager> getSystemManager()
			{
				return system_manager;
			}

		protected :

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			std::function<void()> m_error_callback;

		};

	} // namespace OnlineData
} // namespace WkCocos

#endif // __WKCOCOS_ONLINEDATA_ONLINEDATAMANAGER_H__