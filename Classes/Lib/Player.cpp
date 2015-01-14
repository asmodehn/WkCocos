#include "WkCocos/Player.h"

namespace WkCocos
{
	//constructors
	Player::Player(std::shared_ptr<WkCocos::Timer::Timer> timer,std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb)
		: m_timer(timer)
		, m_localdata(localdata)
		, m_inventory(std::make_shared<WkCocos::Shop::Inventory>())
		, m_pw_gen_cb(pw_gen_cb)
		, m_loggingin(0)
    {
	}

	Player::Player(std::shared_ptr<WkCocos::Timer::Timer> timer, std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<OnlineData::OnlineDataManager> onlinedata)
		: m_timer(timer)
		, m_localdata(localdata)
		, m_onlinedata(onlinedata)
		, m_inventory(std::make_shared<WkCocos::Shop::Inventory>())
		, m_pw_gen_cb(pw_gen_cb)
		, m_loggingin(0)
	{
		m_onlinedata->getEventManager()->subscribe<WkCocos::OnlineData::Events::Error>(*this);
	}

	Player::~Player()
	{

	}

	void Player::autologin()
	{
		++m_loggingin;

		//tried to read existing login data.
		m_localdata->loadLoginID([=](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				setUser(user, passwd);
				newPlayer = false;
			}
			else
			{
				createNewUserID(user, passwd);

				if (user.length() > 0 && passwd.length() > 0)
				{
					setUser(user, passwd);
					//store unique ID
					m_localdata->saveLoginID(m_user, m_passwd, "l0g1nS3cr3tK3y"); //TODO : encyrpt this

					newPlayer = true;
				}
			}
			//in any case we have here m_user != ""

			if (m_onlinedata) //in case online data is set while we re loading loginID
			{
				m_onlinedata->getServerTime([=](std::string s_iso8601){
					m_timer->setTime(s_iso8601);
				});
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
                        events()->emit<LoggedIn>(getId(), m_user);
					});
				}
			}
			else
			{
                events()->emit<LoggedIn>(getId(), m_user);
			}

		}, "l0g1nS3cr3tK3y");
	}

	bool Player::getUsersKeyValue(std::string saveName, std::string key, int value, int quantity, int offset)
	{
		if (m_onlinedata)
		{
			m_onlinedata->getUsersKeyValue(saveName, key, value, quantity, offset);
			return true;
		}
		else
			return false;
	}

	bool Player::getUsersFromTo(std::string saveName, std::string key, int from, int to, int quantity, int offset)
	{
		if (m_onlinedata)
		{
			m_onlinedata->getUsersFromTo(saveName, key, from, to, quantity, offset);
			return true;
		}
		else
			return false;
	}

	bool Player::getAllDocsPaging(std::string saveName, int quantity, int offset)
	{
		if (m_onlinedata)
		{
			m_onlinedata->getAllDocsPaging(saveName, quantity, offset);
			return true;
		}
		else
			return false;
	}

	void Player::receive(const WkCocos::OnlineData::Events::Error& err)
	{

		if (err.httpErrorCode == 401 && err.app42ErrorCode == 1401) //Unauthorized access
		{
			//Major error : we cannot do anything, just trigger an error event.
			//IN COCOS THREAD !!!
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, err](){
				this->events()->emit<Error>(getId(),"app42", ToolBox::itoa(err.httpErrorCode), ToolBox::itoa(err.errorMessage));
			});

		}
		else if (err.httpErrorCode == 404 && err.app42ErrorCode == 2002) //Login existing: Authentication failed
		{
			//Major error : we recreate the user
			std::string newUser;
			std::string newPasswd;

			//TODO : We might want to prompt the user before this.
			createNewUserID(newUser, newPasswd);

			//if new Id is aceptable we replace old one
			if (m_user.length() > 0 && m_passwd.length() > 0)
			{
				setUser(newUser, newPasswd);
				//store unique ID
				m_localdata->saveLoginID(m_user, m_passwd, "l0g1nS3cr3tK3y");
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
				setUser(newUser, newPasswd);
				//store unique ID
				m_localdata->saveLoginID(m_user, m_passwd, "l0g1nS3cr3tK3y");
			}

			//login with new ID ( will create non existing user )
			loginNewUserID();
		}
	}

} //namespace WkCocos
