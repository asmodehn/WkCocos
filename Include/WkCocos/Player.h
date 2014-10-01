#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/Utils/UUID.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Shop/Shop.h"
#include "WkCocos/Timer/Timer.h"
#include "WkCocos/Utils/ToolBox.h"
#include "WkCocos/Save.h"

//using Cocos' Rapidjson for now...
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

#include <string>

namespace WkCocos
{
	/**
	* Hold the player information.
	*/
	class Player : public entityx::Receiver<Player>
	{
	public:

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);
		
		/**
		* Settingup the Inventory of the player
		*/
		void setupInventory(std::shared_ptr<Shop::Inventory> shopInventory);

		/**
		* Setup Timer
		* @param id identifier of the timer
		* @param msecs duration of the timer
		*/
		bool setTimer(std::string id, unsigned long secs)
		{
			struct tm timeinfo = m_timer->getServerUTCTime();
			//CCLOG(asctime(&timeinfo));
			timeinfo.tm_sec += secs;
			
			mktime(&timeinfo);

			//CCLOG(asctime(&timeinfo));
			return m_timer->setAlarm(id, timeinfo);
		}
		
		/**
		* stops Timer
		*/
		void stopTimer(std::string id)
		{
			m_timer->stopAlarm(id);
		}
		std::shared_ptr<Timer::Timer> getTimermgr()
		{
			return  m_timer;
		}
		std::shared_ptr<LocalData::LocalDataManager> getLocalDatamgr()
		{
			return  m_localdata;
		}
		std::shared_ptr<OnlineData::OnlineDataManager> getOnlineDatamgr()
		{
			return  m_onlinedata;
		}

		WkCocos::Shop::Inventory* getInventory()
		{
			return m_inventory.get();
		}

		void receive(const WkCocos::OnlineData::Events::Error& err);

		struct Error : public entityx::Event<Error>
		{
			Error(std::string component, std::string code, std::string message)
			: m_component(component)
			, m_code(code)
			, m_message(message)
			{}

			std::string m_component;
			std::string m_code;
			std::string m_message;
		};

		entityx::ptr<entityx::EventManager> player_events;

		inline const std::string& getUser() const { return m_user; }
		
	protected:
		
		/**
		* constructor for a local player. This will manage local saved data only
		*/
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb);

		/**
		* constructor for an online player. this will manage local saved data and online saved data( these are handled as two separate dataset by WkCocos)
		*/
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<OnlineData::OnlineDataManager> onlinedata, std::function<void()> online_init_cb);

		bool requestLoadData(std::function<void()> loaded_cb, std::string key = "");

		bool requestAllUsers();

		bool requestUsersWithDocs();

		bool requestEnemyData(std::string enemy_data);

		bool requestSaveData(std::function<void()> saved_cb, std::string key = "");
		
		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;
		std::shared_ptr<Timer::Timer> m_timer;
		
		std::shared_ptr<WkCocos::Shop::Inventory> m_inventory;

		//this implements timer save and other data that we manage in this class.
		virtual std::string get_all_data_json();
		virtual void set_all_data_json(std::string data);

		//these are meant to be used by the app for managing extra game data.
		//they are called by get_all_data_json() and set_all_data_json(std::string data)
		virtual std::string get_data_json() = 0;
		virtual void set_data_json(std::string data) = 0;

		Save		m_playerData;
		
	private:

		void createNewUserID(std::string& user, std::string& passwd) const
		{
			//generate unique ID
			std::string user_prefix = "U_";
			std::string uuid = WkCocos::UUID::create();

			if (uuid.length() > 0)
			{
				user = user_prefix + uuid;
				//generating password (deterministic way, so we can recover password later)
				//TODO : this must be set by the game ( so that reading WkCocos code doesnt give passwords away )
				passwd = m_pw_gen_cb(user);
			}
		}

		void loginNewUserID()
		{
			//adding fake email on creation ( email is used for password recovery )
			std::string email = m_user + "@fake.net";

			//autologin. create user and login
			m_onlinedata->loginNew(m_user, m_passwd, email, [=](std::string body){
				CCLOG("login done !!!");
				//loading again to get online value
				m_playerData.requestLoadData(onlineDataLoaded_callback);
			});
		}

		//game callbacks
		std::function<void()> onlineDataLoaded_callback;
		std::function<std::string(std::string userid)> m_pw_gen_cb;

		const char * sAlarms = "alarms";
		const char * sID = "id";

		const char * sSec = "sec";
		const char * sMin = "min";
		const char * sHour = "hour";
		const char * sMday = "mday";
		const char * sMon = "mon";
		const char * sYear = "year";
		const char * sWday = "wday";
		const char * sYday = "yday";
		const char * sIsdst = "isdst";
	};
} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
