#ifndef __WKCOCOS_LOCALDATA_LOCALDATAMANAGER_H__
#define __WKCOCOS_LOCALDATA_LOCALDATAMANAGER_H__

#include "entityx/entityx.h"
#include "WkCocos/LocalData/Events/Error.h"
//#include "json/document.h"

#define DEFAULT_SAVE_VERSION 1

namespace WkCocos
{
	namespace LocalData
	{
		class LocalDataManager
		{
		public:
			/**
			* Constructor 
			*/
			LocalDataManager();

			/**
			* Destructor
			*/
			~LocalDataManager();
			
			/**
			* LoginID
			*/
			bool saveLoginID(std::string user, std::string passwd, std::string key = "", short version = DEFAULT_SAVE_VERSION);
			bool loadLoginID(std::function<void(std::string user, std::string passwd)> load_cb, std::string key = "", short version = DEFAULT_SAVE_VERSION);
			
			/**
			* PlayerData
			*/
			bool savePlayerData(std::string data, std::string key="", short version = DEFAULT_SAVE_VERSION);
			bool loadPlayerData(std::function<void(std::string data)> load_cb, std::string key = "", short version = DEFAULT_SAVE_VERSION);

			void update(double dt);

			inline entityx::ptr<entityx::EventManager> getEventManager() { return event_manager; }

		protected :

			entityx::ptr<entityx::EventManager> event_manager;
			entityx::ptr<entityx::EntityManager> entity_manager;
			entityx::ptr<entityx::SystemManager> system_manager;

			//std::function<void(std::string)> m_error_callback;

		};


	} // namespace LocalData
} // namespace WkCocos



#endif // __WKCOCOS_LOCALDATA_LOCALDATAMANAGER_H__