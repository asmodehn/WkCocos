#ifndef __WKCOCOS_PLAYER_H__
#define __WKCOCOS_PLAYER_H__

#include "cocos2d.h"
#include "WkCocos/LocalData/LocalDataManager.h"

namespace WkCocos
{
	/**
	* Hold the player information.
	* Meyers singleton as the player is there from the beginning, and should available everywhere.
	* we use CRTP design to allow inheritance from it.
	*/
	template<class T>
	class Player
	{
	public:
		static T& GetInstance()
		{
			//careful : this might not be thread safe ...
			m_bSingletonCreating = true;
			static T player;
			m_bSingletonCreating = false;

			return player;
		}

		/**
		* Delayed intialization.
		* Check if saved data exist, and if so it loads it
		*/
		bool Activate();

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);

	protected:
		Player();
		Player(const Player&);
		Player& operator=(const Player&);

		static bool m_bSingletonCreating;

		std::string m_user;
		std::string m_passwd;

		LocalData::LocalDataManager m_localdata;

	};

	// Static variable initialization
	template<class T>
	bool Player<T>::m_bSingletonCreating = false;

	//constructors
	template<class T>
	Player<T>::Player()
	{
	}

	template<class T>
	Player<T>::Player(const Player&)
	{
		return *this;
	}

	//operators
	template<class T>
	Player<T>& Player<T>::operator=(const Player&)
	{
		return *this;
	}

	//member methods
	template<class T>
	bool Player<T>::Activate()
	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player<T>::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");
		
		//tried to read existing data.
		m_localdata.loadLoginID([&](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				m_user = user;
				m_passwd = passwd;
			}
			else
			{
				//generate unique ID
				std::string m_user = "!UID!";
				std::string m_passwd = "!PASS!";
				//store unique ID
				m_localdata.saveLoginID(m_user,m_passwd);
			}


		});

		


		return false;
	}

	template<class T>
	void Player<T>::Update(float deltatime)
	{
		m_localdata.update(deltatime);
	}

} //namespace WkCocos

#endif //__WKCOCOS_PLAYER_H__
