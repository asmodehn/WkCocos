#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/Utils/UUID.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/Shop/Shop.h"
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
	* T is the class that store the player game data.
	* T must be able to serialize and deserialized to/from JSON string.
	*/
	template <class T>
	class Player : public entityx::Receiver<Player<T>>
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

		void receive(const WkCocos::OnlineData::Events::Error& err);

		void openShop();
		void closeShop();

	protected:
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata, std::string licensekey, std::string secretkey, std::shared_ptr<Shop::ShopAssets> shopAssets);

		bool requestLoadData(std::function<void()> loaded_cb);

		bool requestSaveData(std::function<void()> saved_cb);

		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;
		std::shared_ptr<Timer::Timer> m_timer;
		
		std::shared_ptr<WkCocos::Shop::Shop> m_shop;

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

	//constructors
	template <class T>
	Player<T>::Player(std::shared_ptr<LocalData::LocalDataManager> localdata, std::string licensekey, std::string secretkey, std::shared_ptr<Shop::ShopAssets> shopAssets)
		: m_localdata(localdata)
		, m_shop(new WkCocos::Shop::Shop(licensekey, secretkey, shopAssets))
	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player<T>::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");

		m_timer.reset(new WkCocos::Timer::Timer());

		//tried to read existing login data.
		m_localdata->loadLoginID([=](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				m_user = user;
				m_passwd = passwd;

				newPlayer = false;
			}
			else
			{
				createNewUserID(user, passwd);

				if (user.length() > 0 && passwd.length() > 0)
				{
					m_user = user;
					m_passwd = passwd;
					
					//store unique ID
					m_localdata->saveLoginID(m_user, m_passwd);

					newPlayer = true;
				}
			}

			if (m_onlinedata && onlineDataLoaded_callback) //in case online data is set while we re loading loginID
			{
				//we need to do login here
				if (newPlayer)
				{
					loginNewUserID();
				}
				else
				{
					//autologin
					m_onlinedata->login(m_user, m_passwd, [=](std::string body){
						CCLOG("login done !!!");
						//loading again to get online value
						requestLoadData(onlineDataLoaded_callback);
					});
				}
			}
			else
			{
				requestLoadData([](){}); //we assume no callback needed there. we re loading local save.
			}

		});

	}
	
	template <class T>
	void Player<T>::setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata, std::function<void()> online_init_cb)
	{
		m_onlinedata = onlinedata;
		onlineDataLoaded_callback = online_init_cb;

		//subscribing to get error events
		m_onlinedata->getEventManager()->subscribe<WkCocos::OnlineData::Events::Error>(*this);
		
		if (m_user != "")
		{
			if (newPlayer)
			{
				loginNewUserID();
			}
			else
			{
				//autologin
				m_onlinedata->login(m_user, m_passwd, [=](std::string body){
					CCLOG("login done !!!");
					//loading again to get online value
					requestLoadData(onlineDataLoaded_callback);
				});
			}
		}
		else //save not loaded yet
		{
			//nothing to do. it will be called again after save has been loaded
		}
	}
	
	template <class T>
	bool Player<T>::requestLoadData(std::function<void()> loaded_cb)
	{
		if (m_onlinedata)
		{
			m_onlinedata->load(m_user, [=](std::string data)
			{
				set_all_data_json(data);
				CCLOG("user data loaded : %s", data.c_str());
				
				loaded_cb();
			});
			
			return true;
		}
		else if (m_localdata)
		{
			m_localdata->loadPlayerData([=](std::string data){
				set_all_data_json(data);

				loaded_cb();
			});

			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	bool Player<T>::requestSaveData(std::function<void()> saved_cb)
	{
		if (m_onlinedata)
		{
			
			m_onlinedata->save(m_user, get_all_data_json(), [=](std::string data)
			{
				CCLOG("user data saved : %s", data.c_str());
				saved_cb();
			});
			
			return true;
		}
		else if (m_localdata)
		{
			m_localdata->savePlayerData(get_all_data_json());
			saved_cb();
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	void Player<T>::openShop()
	{
		m_shop->activate();
		
	}

	template <class T>
	void Player<T>::closeShop()
	{
		m_shop->deactivate();
	}


	template <class T>
	std::string Player<T>::get_all_data_json()
	{
		rapidjson::Document doc;
		// must pass an allocator when the object may need to allocate memory
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		//calling get_data_json to retrieve extra game data
		std::string extra_data_json = get_data_json();

		doc.Parse<0>(extra_data_json.c_str());
		if (doc.HasParseError())
		{
			//if parse error (also empty string), we ignore existing data.
			doc.SetObject();
		}

		rapidjson::Value alarms;
		alarms.SetArray();

		entityx::ptr<WkCocos::Timer::Comp::ID> id;
		entityx::ptr<WkCocos::Timer::Comp::Alarm> alarm;
		for (auto entity : m_timer->getEntityManager()->entities_with_components(id, alarm))
		{
			rapidjson::Value time;
			time.SetObject();
			time.AddMember(sID, id->m_id.c_str(), allocator);

			time.AddMember(sSec, alarm->m_end.tm_sec, allocator);
			time.AddMember(sMin, alarm->m_end.tm_min, allocator);
			time.AddMember(sHour, alarm->m_end.tm_hour, allocator);
			time.AddMember(sMday, alarm->m_end.tm_mday, allocator);
			time.AddMember(sMon, alarm->m_end.tm_mon, allocator);
			time.AddMember(sYear, alarm->m_end.tm_year, allocator);
			time.AddMember(sWday, alarm->m_end.tm_wday, allocator);
			time.AddMember(sYday, alarm->m_end.tm_yday, allocator);
			time.AddMember(sIsdst, alarm->m_end.tm_isdst, allocator);

			alarms.PushBack(time, allocator);
		}
		doc.AddMember(sAlarms, alarms, allocator);

		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		doc.Accept(writer);

		return std::string(strbuf.GetString());
		
	}

	template <class T>
	void Player<T>::set_all_data_json(std::string data)
	{
		rapidjson::Document doc;
		doc.Parse<0>(data.c_str());
		if (doc.HasParseError())
		{
			//if parse error (also empty string), we ignore existing data.
			doc.SetObject();
		}

		if (doc.HasMember(sAlarms))
		{
			rapidjson::Value& alarmsarray = doc[sAlarms];
			if (alarmsarray.Size()){
				for (rapidjson::SizeType i = 0; i < alarmsarray.Size(); i++)
				{
					rapidjson::Value& time = alarmsarray[i];

					struct tm temptm;
					temptm.tm_hour = time[sHour].GetInt();
					temptm.tm_isdst = time[sIsdst].GetInt();
					temptm.tm_mday = time[sMday].GetInt();
					temptm.tm_min = time[sMin].GetInt();
					temptm.tm_mon = time[sMon].GetInt();
					temptm.tm_sec = time[sSec].GetInt();
					temptm.tm_wday = time[sWday].GetInt();
					temptm.tm_yday = time[sYday].GetInt();
					temptm.tm_year = time[sYear].GetInt();

					m_timer->setAlarm(time[sID].GetString(), temptm);
				}
			}
			doc.RemoveMember(sAlarms);
		}

		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		doc.Accept(writer);

		return set_data_json(strbuf.GetString());
	}


	template <class T>
	void Player<T>::Update(float deltatime)
	{
		if (m_timer)
		{
			m_timer->update(deltatime);
		}
		if (m_localdata)
		{
			m_localdata->update(deltatime);
		}
		if (m_onlinedata)
		{
			m_onlinedata->update(deltatime);
		}
	}

	template <class T>
	void Player<T>::receive(const WkCocos::OnlineData::Events::Error& err)
	{
		if (err.httpErrorCode == 404 && err.app42ErrorCode == 2002 ) //Login existing: Authentication failed
		{
			//Major error : we recreate the user
			std::string newUser;
			std::string newPasswd;

			//TODO : We might want to prompt the user before this.
			createNewUserID(newUser, newPasswd);

			//if new Id is aceptable we replace old one
			if (m_user.length() > 0 && m_passwd.length() > 0)
			{
				m_user = newUser;
				m_passwd = newPasswd;

				//store unique ID
				m_localdata->saveLoginID(m_user, m_passwd);
			}
			
			//login with new ID ( will create non existing user )
			loginNewUserID();
		}
		else if (err.httpErrorCode == 400 && err.app42ErrorCode == 2001) //Creation failed : Username already exist
		{
			//we recreate the user
			std::string newUser;
			std::string newPasswd;

			createNewUserID(newUser, newPasswd);

			//if new Id is aceptable we replace old one
			if (m_user.length() > 0 && m_passwd.length() > 0)
			{
				m_user = newUser;
				m_passwd = newPasswd;

				//store unique ID
				m_localdata->saveLoginID(m_user, m_passwd);
			}

			//login with new ID ( will create non existing user )
			loginNewUserID();
		}
	}

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
