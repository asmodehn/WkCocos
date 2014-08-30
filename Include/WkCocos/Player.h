#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/Utils/UUID.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Timer/Timer.h"
#include "WkCocos/Utils/ToolBox.h"

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
		* Player by default maintain a local save.
		* After online data manager is set, the local save is not used anymore and the online save takes priority.
		*/
		void setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata, std::function<void()> online_init_cb);
		
		/**
		* Setup Timer
		* @param id identifier of the timer
		* @param msecs duration of the timer
		*/
		bool setTimer(std::string id, unsigned long secs)
		{
			struct tm timeinfo = ToolBox::getUTCTime();
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

		void receive(const WkCocos::OnlineData::Events::Error& err);

	protected:
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata);

		bool requestLoadData(std::function<void()> loaded_cb);

		bool requestAllUsers();

		bool requestUsersWithDocs();

		bool requestEnemyData(std::string enemy_data);

		bool requestSaveData(std::function<void()> saved_cb);

		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;
		std::shared_ptr<Timer::Timer> m_timer;

		//this implements timer save and other data that we manage in this class.
		virtual std::string get_all_data_json();
		virtual void set_all_data_json(std::string data);

		//these are meant to be used by the app for managing extra game data.
		//they are called by get_all_data_json() and set_all_data_json(std::string data)
		virtual std::string get_data_json() = 0;
		virtual void set_data_json(std::string data) = 0;


		
	private:

		void createNewUserID(std::string& user, std::string& passwd) const
		{
			//generate unique ID
			std::string user_prefix = "fake_";
			std::string uuid = WkCocos::UUID::create();

			if (uuid.length() > 0)
			{
				user = user_prefix + uuid;
				//generating password (deterministic way, so we can recover password later)
				//TODO
				passwd = uuid;
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
				requestLoadData(onlineDataLoaded_callback);
			});
		}

		//game callbacks
		std::function<void()> onlineDataLoaded_callback;

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

	/*	m_localdata->loadPlayerData([=](std::string data)
		{
			rapidjson::Document doc;
			doc.Parse<0>(data.c_str());
			if (doc.HasParseError())
			{
				//if parse error (also empty string), we ignore existing data.
				doc.SetObject();
			}
			set_data_json(doc);
		});*/


	/*	// create document
		rapidjson::Document doc;
		doc.SetObject();
		// must pass an allocator when the object may need to allocate memory
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		get_data_json(doc, allocator);

		// Get the save string
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		doc.Accept(writer);

		std::string save = std::string(strbuf.GetString());

		m_localdata->savePlayerData(save);*/
} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
