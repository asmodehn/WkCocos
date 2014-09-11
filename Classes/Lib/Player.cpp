#include "WkCocos/Player.h"

namespace WkCocos
{
	//constructors
	Player::Player(std::shared_ptr<LocalData::LocalDataManager> localdata, std::shared_ptr<Shop::Inventory> shopInventory, Save::Mode mode)
		: m_localdata(localdata)
		, m_inventory(shopInventory)
		, m_playerData("user_data", mode)
		, player_events(entityx::EventManager::make())

	{
		//registering player class in cocos update loop
		cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Player::Update, this, std::placeholders::_1), this, 1.f / 15, false, "player_update");

		m_timer.reset(new WkCocos::Timer::Timer());

		// Init Save
		m_playerData.registerLoadingCallback(std::bind(&Player::set_all_data_json, this, std::placeholders::_1));
		m_playerData.registerSavingCallback(std::bind(&Player::get_all_data_json, this));
		m_playerData.setLocalDataMgr(m_localdata);

		//tried to read existing login data.
		m_localdata->loadLoginID([=](std::string user, std::string passwd){
			if (user != "" && passwd != "")
			{
				m_user = user;
				m_passwd = passwd;
				m_playerData.setUserName(m_user);

				newPlayer = false;
			}
			else
			{
				createNewUserID(user, passwd);

				if (user.length() > 0 && passwd.length() > 0)
				{
					m_user = user;
					m_passwd = passwd;
					m_playerData.setUserName(m_user);

					//store unique ID
					m_localdata->saveLoginID(m_user, m_passwd, "l0g1nS3cr3tK3y" ); //TODO : encyrpt this

					newPlayer = true;
				}
			}

			if (m_onlinedata && onlineDataLoaded_callback) //in case online data is set while we re loading loginID
			{
				m_onlinedata->getServerTime([=](std::string s_iso8601){
					m_timer->setRunTime(s_iso8601);
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
						//loading again to get online value
						m_playerData.requestLoadData(onlineDataLoaded_callback);
					});
				}
			}
			else
			{
				m_playerData.requestLoadData([](){}); //we assume no callback needed there. we re loading local save.
			}

		}, "l0g1nS3cr3tK3y");

	}

	void Player::setOnlineDataManager(std::shared_ptr<OnlineData::OnlineDataManager> onlinedata, std::function<void()> online_init_cb)
	{
		m_onlinedata = onlinedata;
		m_playerData.setOnlineDataMgr(m_onlinedata);
		onlineDataLoaded_callback = online_init_cb;

		//subscribing to get error events
		m_onlinedata->getEventManager()->subscribe<WkCocos::OnlineData::Events::Error>(*this);

		if (m_user != "") // this seems to be allways empty after save class implementation
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
					m_playerData.requestLoadData(onlineDataLoaded_callback);
				});
			}
		}
		else //save not loaded yet
		{
			//nothing to do. it will be called again after save has been loaded
		}
	}

	bool Player::requestAllUsers()
	{
		if (m_onlinedata)
		{
			m_onlinedata->getAllUsers();
			return true;
		}
		else
			return false;
	}

	bool Player::requestUsersWithDocs()
	{
		if (m_onlinedata)
		{
			m_onlinedata->getUsersWithDocs();
			return true;
		}
		else
			return false;
	}

	bool Player::requestEnemyData(std::string enemy_data)
	{
		if (m_onlinedata)
		{
			m_onlinedata->loadEnemy(enemy_data, m_playerData.getSaveName());
			return true;
		}
		else
			return false;
	}

	//bool Player::requestServerTime()
	//{
	//	if (m_onlinedata)
	//	{
	//		m_onlinedata->getServerTime();
	//		return true;
	//	}
	//	else
	//		return false;
	//}

	std::string Player::get_all_data_json()
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

		//test
		//rapidjson::Value playerData;
		//playerData.SetArray();
		//size = 446 bytes
		//const char * fillerText = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
		//i < 9 causes crash somwhere in heap malloc, so 8*446=3.5 kb can be saved normally
		//14.4 kb Julien's data file still can not be tested
		//for (int i = 0; i < 8; i++)
		//	playerData.PushBack(fillerText, allocator);
		//doc.AddMember("playerData", playerData, allocator);

		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		doc.Accept(writer);

		std::string data = std::string(strbuf.GetString());
		
		//TEST DATA BEGIN
		//std::string data_filename = cocos2d::FileUtils::getInstance()->fullPathForFilename("LocalData.txt");
		//std::ifstream datafile(data_filename);
		//if (datafile)
		//{
		//	std::stringstream strbuffer;
		//	strbuffer << datafile.rdbuf();
		//	data = strbuffer.str();
		//}
		//TEST DATA END

		return data;

	}

	void Player::set_all_data_json(std::string data)
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

	void Player::Update(float deltatime)
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

	void Player::receive(const WkCocos::OnlineData::Events::Error& err)
	{

		if (err.httpErrorCode == 401 && err.app42ErrorCode == 1401) //Unauthorized access
		{
			//Major error : we cannot do anything, just trigger an error event.
			//IN COCOS THREAD !!!
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, err](){
				this->player_events->emit<Error>("app42", ToolBox::itoa(err.httpErrorCode), ToolBox::itoa(err.errorMessage));
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
