#include "WkCocosApp/MyPlayer.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

#define SAVENAME "MySave"

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::Timer::Timer> timermgr, std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::function<std::string(std::string userid)> pw_gen_cb)
	: m_player(timermgr, localdatamngr, pw_gen_cb)
	, m_save(SAVENAME, WkCocos::Save::Mode::ONLINE)
	, m_gem("53cr3t") // encrypted
	, m_gold() // not encrypted
	, m_loggingIn(false)
{
	m_gem.set(42);
	m_gold.set(424242);

	m_save.setLocalDataMgr(m_player.getLocalDatamgr());
	m_save.setOnlineDataMgr(m_player.getOnlineDatamgr());
	WkCocos::Save::getEventManager()->subscribe<WkCocos::Save::Loaded>(*this);
	WkCocos::Save::getEventManager()->subscribe<WkCocos::Save::Saved>(*this);
}

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::Timer::Timer> timermgr, std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinedatamgr)
	: m_player(timermgr, localdatamngr, pw_gen_cb, onlinedatamgr)
	, m_save(SAVENAME, WkCocos::Save::Mode::ONLINE)
, m_gem("53cr3t") // encrypted
, m_gold() // not encrypted
, m_loggingIn(false)
{
	m_gem.set(42);
	m_gold.set(424242);
	
	m_save.setLocalDataMgr(m_player.getLocalDatamgr());
	m_save.setOnlineDataMgr(m_player.getOnlineDatamgr());
	WkCocos::Save::getEventManager()->subscribe<WkCocos::Save::Loaded>(*this);
	WkCocos::Save::getEventManager()->subscribe<WkCocos::Save::Saved>(*this);
}

MyPlayer::~MyPlayer()
{
}

void MyPlayer::login()
{
	m_loggingIn = true;
	m_player.getEventManager()->subscribe<WkCocos::Player::LoggedIn>(*this);
	m_player.autologin();
}

//save Data for test
void MyPlayer::saveData()
{
	m_player.saveData();

	rapidjson::Document doc;
	doc.SetObject();
	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value currency;
	currency.SetObject();
	currency.AddMember(sGold, m_gold.get<int>(), allocator);
	currency.AddMember(sGem, m_gem.get<int>(), allocator);
	doc.AddMember(sCurrency, currency, allocator);

	//TMP debug
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	doc.Accept(writer);

	m_save.requestSaveData(strbuf.GetString());
}

//load Data for test
void MyPlayer::loadData()
{
	m_player.loadData();
	m_save.requestLoadData();
}

bool MyPlayer::getAllDocsPaging(int quantity, int offset)
{
	return m_player.getAllDocsPaging(SAVENAME, quantity, offset);
}

bool MyPlayer::getUsersFromTo(std::string key, int from, int to, int quantity, int offset)
{
	return m_player.getUsersFromTo(SAVENAME, key, from, to, quantity, offset);
}


void MyPlayer::receive(const WkCocos::Player::LoggedIn& loggedin)
{
	m_save.setUserName(loggedin.m_username);
	m_save.requestLoadData();
}

void MyPlayer::receive(const WkCocos::Save::Loaded& loaded_evt)
{
	if (m_save.getName() == loaded_evt.m_name)
	{
		rapidjson::Document doc;

		//we get data
		if (m_save.getData().empty())
		{
			doc.SetObject();
		}
		else
		{
			doc.Parse<0>(m_save.getData().c_str());
			if (doc.HasParseError())
			{
				//if parse error (also empty string), we ignore existing data.
				doc.SetObject();
			}
		}

		if (doc.HasMember(sCurrency))
		{
			rapidjson::Value& currencyvalue = doc[sCurrency];
			if (!currencyvalue.IsNull()){
				if (currencyvalue.HasMember(sGold) && currencyvalue[sGold].IsInt())
				{
					m_gold.set<int>(currencyvalue[sGold].GetInt());
				}
				if (currencyvalue.HasMember(sGem) && currencyvalue[sGem].IsInt())
				{
					m_gem.set<int>(currencyvalue[sGem].GetInt());
				}
			}
		}

		if (m_loggingIn)
		{
			getEventManager()->emit<MyPlayer::LoggedIn>(getId());
			m_loggingIn = false;
		}

		getEventManager()->emit<MyPlayer::Loaded>(getId());
	}
}

void MyPlayer::receive(const WkCocos::Save::Saved& saved_evt)
{
	if (m_save.getName() == saved_evt.m_name)
	{
		getEventManager()->emit<MyPlayer::Saved>(getId());
	}
}

