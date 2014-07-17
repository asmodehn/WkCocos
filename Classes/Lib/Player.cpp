#include "WkCocos/Player.h"

namespace WkCocos
{

	//constructors
	Player::Player(std::shared_ptr<LocalData::LocalDataManager> localdata)
		: m_localdata(localdata)
	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");

		//tried to read existing data.
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
	}

	void Player::setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata)
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
				});
			}
			else
			{
				//autologin
				m_onlinedata->login(m_user, m_passwd, [=](void * data){
					CCLOG("login done !!!");
				});
			}
		}
		else //save not loaded yet
		{

		}
	}

	void Player::Update(float deltatime)
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

}
