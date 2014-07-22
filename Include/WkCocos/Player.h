#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/Utils/UUID.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

#include <string>

namespace WkCocos
{
	/**
	* Hold the player information.
	* T is the class that store the player game data.
	* T must be able to serialize and deserialized to/from JSON string.
	*/
	template <class T>
	class Player
	{
	public:

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);
		
		void setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata);
		
	protected:
		Player(std::shared_ptr<LocalData::LocalDataManager> localdata);

		bool requestLoadData();

		bool requestSaveData();

		bool newPlayer;
		std::string m_user;
		std::string m_passwd;

		std::shared_ptr<LocalData::LocalDataManager> m_localdata;
		std::shared_ptr<OnlineData::OnlineDataManager> m_onlinedata;

		virtual std::string get_data_json() = 0;
		virtual void set_data_json(std::string data) = 0;
	};

	//constructors
	template <class T>
	Player<T>::Player(std::shared_ptr<LocalData::LocalDataManager> localdata)
		: m_localdata(localdata)
	{
		//registering player class in cocos update loop

		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player<T>::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");

		//tried to read existing login data.
		m_localdata->loadLoginID([=](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				m_user = user;
				m_passwd = passwd;

				newPlayer = false;

				if (m_onlinedata)
				{
					//reset online data manager to force login
					setOnlineDataManager(m_onlinedata);
				}
			}
			else
			{
				//generate unique ID
				std::string user_prefix = "fake_";
				std::string uuid = WkCocos::UUID::create();

				user = user_prefix + uuid;
				//generating password
				passwd = uuid;

				//generating password (deterministic way, so we can recover password later)
				//TODO

				if (user.length() > 0 && passwd.length() > 0)
				{
					m_user = user;
					m_passwd = passwd;

					//store unique ID
					m_localdata->saveLoginID(m_user, m_passwd);

					newPlayer = true;
					if (m_onlinedata)
					{
						//reset online data manager to force login
						setOnlineDataManager(m_onlinedata);
					}
				}
			}

		});

		m_localdata->loadPlayerData([=](std::string data){

			set_data_json(data);

		});
	}
	
	template <class T>
	void Player<T>::setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata)
	{
		m_onlinedata = onlinedata;

		if (m_user != "")
		{
			if (newPlayer)
			{
				//adding fake email on creation ( email is used for password recovery )
				std::string email = m_user + "@fake.net";

				//autologin. create user and login
				m_onlinedata->loginNew(m_user, m_passwd, email, [=](void * data){
					CCLOG("login done !!!");

					requestLoadData();
				});
			}
			else
			{
				//autologin
				m_onlinedata->login(m_user, m_passwd, [=](void * data){
					CCLOG("login done !!!");

					requestLoadData();
				});
			}
		}
		else //save not loaded yet
		{
			//nothing to do. it will be called again after save has been loaded
		}
	}
	
	template <class T>
	bool Player<T>::requestLoadData()
	{
		if (m_onlinedata)
		{
			//m_onlinedata->load(m_user, [=](void* data)
			//{
			//	CCLOG("profile loaded !!!");
			//	//TODO : decide if we keep local or online data
			//});
			return true;
		}
		else
		{
			return false;
		}


	}

	template <class T>
	bool Player<T>::requestSaveData()
	{
		m_localdata->savePlayerData(get_data_json());

		if (m_onlinedata)
		{
			//m_onlinedata->save(m_user, get_data_json(), [=](void* data)
			//{
			//	CCLOG("profile saved !!!");
			//	//TODO : decide if we keep local or online data
			//});
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	void Player<T>::Update(float deltatime)
	{
		if (m_localdata)
		{
			m_localdata->update(deltatime);
		}
		if (m_onlinedata)
		{
			m_onlinedata->update(deltatime);
		}
	}

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
