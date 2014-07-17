#include "WkCocos/Player.h"

namespace WkCocos
{

	//constructors
	Player::Player(std::string app_access_key, std::string app_secret_key)
		: m_localdata(std::bind(&Player::local_save_error_callback, this))
		, m_onlinedata(app_access_key, app_secret_key, std::bind(&Player::online_save_error_callback, this))
	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");

		//tried to read existing data.
		m_localdata.loadLoginID([=](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				this->m_user = user;
				this->m_passwd = passwd;

				//autologin
				m_onlinedata.login(this->m_user, this->m_passwd, [=](void * data){
					CCLOG("login done !!!");
				});
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
					this->m_user = user;
					this->m_passwd = passwd;

					//store unique ID
					m_localdata.saveLoginID(this->m_user, this->m_passwd);

					//adding fake email on creation ( email is used for password recovery )
					std::string email = this->m_user + "@fake.net";

					//autologin. create user and login
					m_onlinedata.loginNew(this->m_user, this->m_passwd, email, [=](void * data){
						CCLOG("login done !!!");
					});
				}
			}

		});
	}

	//member methods
	void Player::Update(float deltatime)
	{
		m_localdata.update(deltatime);
		m_onlinedata.update(deltatime);
	}

	void Player::local_save_error_callback()
	{
		CCLOG("LOCAL SAVE ERROR");
	}

	void Player::online_save_error_callback()
	{
		CCLOG("ONLINE SAVE ERROR");
	}

}
