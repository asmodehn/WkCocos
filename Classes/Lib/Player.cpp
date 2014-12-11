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
		// Saves
		//Save* m_moreData = new Save(moreSaveName, Save::Mode::OFFLINE);
		//Save* m_timerData = new Save(timerSaveName, Save::Mode::OFFLINE);
		//m_moreData->setLocalDataMgr(m_localdata);
		//m_timerData->setLocalDataMgr(m_localdata);
		//m_save.insert(make_pair(moreSaveName, m_moreData));
		//m_save.insert(make_pair(timerSaveName, m_timerData));
		//Save::getEventManager()->subscribe<Save::Loaded>(*this);
		//Save::getEventManager()->subscribe<Save::Saved>(*this);
	}

	Player::Player(std::shared_ptr<WkCocos::Timer::Timer> timer, std::shared_ptr<LocalData::LocalDataManager> localdata, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<OnlineData::OnlineDataManager> onlinedata)
		: m_timer(timer)
		, m_localdata(localdata)
		, m_onlinedata(onlinedata)
		, m_inventory(std::make_shared<WkCocos::Shop::Inventory>())
		, m_pw_gen_cb(pw_gen_cb)
		, m_loggingin(0)
	{
		// Saves
		//Save* timerData = new Save(timerSaveName, Save::Mode::ONLINE);
		//timerData->setLocalDataMgr(m_localdata);
		//timerData->setOnlineDataMgr(m_onlinedata);
		//m_save.insert(make_pair(timerSaveName, timerData));
		//Save::getEventManager()->subscribe<Save::Loaded>(*this);
		//Save::getEventManager()->subscribe<Save::Saved>(*this);

		//Exemple of additional save for player :
		//Save* moreData = new Save(moreSaveName, Save::Mode::ONLINE);
		//moreData->setLocalDataMgr(m_localdata);
		//moreData->setOnlineDataMgr(m_onlinedata);
		//m_save.insert(make_pair(moreSaveName, moreData));

		Save::getEventManager()->subscribe<Save::Loaded>(*this);
		Save::getEventManager()->subscribe<Save::Saved>(*this);

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
						loadData();
					});
				}
			}
			else
			{
				loadData();
			}

		}, "l0g1nS3cr3tK3y");
	}

	void Player::receive(const WkCocos::Save::Saved& saved_evt)
	{
		//checking if all saves are loaded
		bool all_saved(true);
		for (auto save : m_save)
		{
			if (!save.second->isSaved())
			{
				all_saved = false;
				break;
			}
		}

		//if all saves loaded, we can trigger loaded event.
		if (all_saved)
		{
			events()->emit<Saved>(getId());
		}
	}

	void Player::receive(const WkCocos::Save::Loaded& loaded_evt)
	{
		auto saveit = m_save.find(loaded_evt.m_name);
		if (m_save.end() != saveit)
		{
			auto save = saveit->second;
			if (timerSaveName == save->getName())
			{
				rapidjson::Document doc;

				//all string are supposed to contain similar data.
				//we only care about last one ( most recent ? )
				if (save->getData().empty())
				{
					doc.SetObject();
				}
				else
				{
					doc.Parse<0>(save->getData().c_str());
					if (doc.HasParseError())
					{
						//if parse error (also empty string), we ignore existing data.
						doc.SetObject();
					}
				}

				if (doc.HasMember(sAlarms))
				{
					rapidjson::Value& alarmsarray = doc[sAlarms];
					if (alarmsarray.Size())
					{
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
				}

			}
			else if (moreSaveName == save->getName())
			{
				//TODO : fill in with more useful data
			}

            //checking if all saves are loaded
            bool all_loaded(true);
			for ( auto save : m_save)
			{
				if (!save.second->isLoaded())
				{
					all_loaded = false;
					break;
				}
			}

			//if all saves loaded, we can trigger loaded event.
			if (all_loaded)
			{
				std::map<std::string, std::string> data;
				for (auto save : m_save)
				{
					data.insert(make_pair(save.first, save.second->getData()));
				}
				events()->emit<Loaded>(getId());
				if (0 == --m_loggingin)
				{
					//if all login requests have replied we are logged in.
					events()->emit<LoggedIn>(getId(), m_user);
				}
			}

		}
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


	/**
	* request a Save
	*/
	bool Player::saveData()
	{
		bool saved = true;
		for (auto save : m_save)
		{
			//init json doc
			rapidjson::Document doc;
			rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
			doc.SetObject();
			//prepare string buffer
			rapidjson::StringBuffer strbuf;
			rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);

			if (timerSaveName == save.first)
			{
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
				//doc.Accept(writer);

				save.second->getEventManager()->subscribe<Save::Saved>(*this);
				//save.second->requestSaveData(strbuf.GetString());
			}
			else if (moreSaveName == save.first)
			{
				//TODO : fill in with more useful data
			}

			//save json string
			doc.Accept(writer);
			saved = save.second->requestSaveData(strbuf.GetString()) && saved ; //keep saving even if one fails.
		}
		return saved;
	}

	/**
	* request a Load
	*/
	bool Player::loadData()
	{
		bool loaded = true;
		if (m_save.size() > 0)
		{
			for (auto save : m_save)
			{
				loaded = save.second->requestLoadData() && loaded;
			}
		}
		else
		{
			//no save, just log in
			events()->emit<LoggedIn>(getId(), m_user);
		}
		return loaded;
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
