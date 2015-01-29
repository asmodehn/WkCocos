#ifndef __MYPLAYER_H__
#define __MYPLAYER_H__

#include "WkCocos/Player.h"

#include "WkCocos/StrongBox/StrongBox.h"

#include "WkCocos/Shop/Inventory.h"

#include "WkCocos/Utils/GPGSManager.h"

/**
* This is the game player
*/
class MyPlayer : public WkCocos::Actor
{
	//delegate
	WkCocos::Player m_player;

public:

	std::shared_ptr<entityx::EventManager> getEventManager()
	{
		return m_player.getEventManager();
	}

	/**
	* Constructor
	*/
	MyPlayer(std::shared_ptr<WkCocos::Timer::Timer> gameclock, std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::function<std::string(std::string userid)> pw_gen_cb);
	MyPlayer(std::shared_ptr<WkCocos::Timer::Timer> gameclock, std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinedatamgr);
	/**
	* Destructor
	*/
	virtual ~MyPlayer();

	/**
	* event receivers
	*/
	void receive(const WkCocos::Player::LoggedIn& constructed);
	void receive(const WkCocos::Player::Error &PL);
	void receive(const WkCocos::Save::Loaded& loaded);
	void receive(const WkCocos::Save::Saved& saved);
	void receive(const WkCocos::Save::Error & save_err);

    void receive(const GPGSManager::SnapshotLoaded & snaploaded);

	//ingame currency
	WkCocos::StrongBox::StrongBox m_gold;
	//premium currency
	WkCocos::StrongBox::StrongBox m_gem;

	//login for test
	void login();

	//save Data for test
	void saveData(bool snapshot = false);

	//load Data for test
	void loadData(bool snapshot = false);


	struct LoggedIn : public WkCocos::Event < LoggedIn >
	{
		LoggedIn(WkCocos::ActorID id)
			: WkCocos::Event< LoggedIn >(id)
		{}
	};

	struct Loaded : public WkCocos::Event < Loaded >
	{
		Loaded(WkCocos::ActorID id)
			: WkCocos::Event< Loaded >(id)
		{}
	};

	struct Saved : public WkCocos::Event < Saved >
	{
		Saved(WkCocos::ActorID id)
			: WkCocos::Event< Saved >(id)
		{}
	};

	struct Error : public WkCocos::Event < Error >
	{
		Error(WkCocos::ActorID id, std::string msg)
			: WkCocos::Event< Error >(id)
			, message(msg)
		{}

		std::string message;
	};

	WkCocos::Shop::Inventory* getInventory()
	{
		return m_player.getInventory();
	}

	void setupInventory(std::shared_ptr<WkCocos::Shop::Inventory> invent)
	{
		m_player.setupInventory(invent);
	}

	bool getAllDocsPaging(int quantity, int offset);

	bool getUsersKeyValue(std::string key, int value, int quantity, int offset);

	bool getUsersFromTo(std::string key, int from, int to, int quantity, int offset);

	std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> getOnlineDatamgr()
	{
		return m_player.getOnlineDatamgr();
	}

	std::shared_ptr<WkCocos::Timer::Timer> getTimermgr()
	{
		return m_player.getTimermgr();
	}

	bool setTimer(std::string id, unsigned long secs )
	{
		return m_player.setTimer(id, secs);
	}

	void stopTimer(std::string id)
	{
		m_player.stopTimer(id);
	}

private:

	const char * sCurrency = "currency";
	const char * sGem = "gem";
	const char * sGold = "gold";

    void setData(std::string d);

	WkCocos::Save m_save;

	bool m_loggingIn;

};
#endif // __MYPLAYER_H__
