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
#include "WkCocos/Actor.h"

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
	class Player : public Actor, public entityx::Receiver<Player>
	{
	public:
		
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

		struct Error : public Event<Error>
		{
			Error(ActorID id, std::string component, std::string code, std::string message)
				: Event(id)
			, m_component(component)
			, m_code(code)
			, m_message(message)
			{}

			std::string m_component;
			std::string m_code;
			std::string m_message;
		};

		struct LoggedIn : public Event < LoggedIn >
		{
			LoggedIn(ActorID id, std::string username)
				: Event(id)
				, m_username(username)
			{}

			std::string m_username;
		};

		struct Loaded : public Event < Loaded >
		{
			Loaded(ActorID id)
				: Event(id)
			{}
		};

		struct Saved : public Event < Saved >
		{
			Saved(ActorID id)
				: Event(id)
			{}
		};

		
		inline const std::string& getUser() const { return m_user; }

		bool getUsersKeyValue(std::string saveName, std::string key, int value, int quantity, int offset);

		bool getUsersFromTo(std::string saveName, std::string key, int from, int to, int quantity, int offset);

		bool getAllDocsPaging(std::string saveName, int quantity, int offset);
				
		/**
		* constructor for a local player. This will manage local saved data only
		*/
		Player(std::shared_ptr<WkCocos::Timer::Timer> timer, std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb);

		/**
		* constructor for an online player. this will manage local saved data and online saved data( these are handled as two separate dataset by WkCocos)
		* TODO :
		*       Login is now separated from player constructor : we do not need the online constructor here anymore.
		*       A player can "become online" as soon as he does the login successfully.
		*/
		Player(std::shared_ptr<WkCocos::Timer::Timer> timer, std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<OnlineData::OnlineDataManager> onlinedata);

		/**
		* destructor
		*/
		~Player();

		/**
		* request a Login
		*/
		void autologin();

		bool isLoggedIn()
		{
			return 0 == m_loggingin;
		}

		/**
		* request a Save
		*/
		void saveData();

		/**
		* request a Load
		*/
		void loadData();

		/**
		* Receive data loaded
		*/
		void receive(const WkCocos::Save::Loaded& constructed_evt);

		/**
		* Receive data saved
		*/
		void receive(const WkCocos::Save::Saved& saved_evt);

	protected:
		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;
		std::shared_ptr<Timer::Timer> m_timer;
		
		std::shared_ptr<WkCocos::Shop::Inventory> m_inventory;

		//map of saves indexed based on name.
		std::map<std::string,Save*> m_save;

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
				loadData();
			});
		}

		//synchronous callbacks
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

		const std::string timerSaveName = "timer";
		const std::string moreSaveName = "more";

		//requests for logging in
		unsigned short m_loggingin;
	};
} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
