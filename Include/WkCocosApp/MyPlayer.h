#ifndef __MYPLAYER_H__
#define __MYPLAYER_H__

#include "WkCocos/Player.h"

#include "WkCocos/StrongBox/StrongBox.h"

#include "WkCocos/Shop/Inventory.h"


/**
* This is the game player
*/
class MyPlayer : public WkCocos::Player
{
public:

	/**
	* Constructor
	*/
	MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::function<std::string(std::string userid)> pw_gen_cb, std::shared_ptr<WkCocos::OnlineData::OnlineDataManager> onlinedatamgr, std::function<void()> online_init_cb);
	/**
	* Destructor
	*/
	virtual ~MyPlayer();
	

	/**
	* data accessor as json string
	*/
	std::string get_data_json();

	/**
	* data setter from json string
	*/
	void set_data_json(std::string data);

	//ingame currency
	WkCocos::StrongBox::StrongBox m_gold;
	//premium currency
	WkCocos::StrongBox::StrongBox m_gem;

	//save Data for test
	void saveData(std::function<void()> saved_cb)
	{
		m_playerData.requestSaveData(saved_cb);
	}

	//load Data for test
	void loadData(std::function<void()> loaded_cb)
	{
		m_playerData.requestLoadData(loaded_cb);
	}

	//load enemy for test
	void loadEnemy(std::string enemy_name)
	{
		requestEnemyData(enemy_name);
	}
	
	//test for getting users with at least one saved doc
	//void getUsersWithDocs()
	//{
	//	requestUsersWithDocs();
	//}

	void getUsersKeyValue(std::string key, int value, int quantity, int offset)
	{
		requestUsersKeyValue(key, value, quantity, offset);
	}

	void getUsersFromTo(std::string key, int from, int to, int quantity, int offset)
	{
		requestUsersFromTo(key, from, to, quantity, offset);
	}

	//test for getting server time
	//void getServerTime()
	//{
	//	requestServerTime();
	//}

private:

	const char * sCurrency = "currency";
	const char * sGem = "gem";
	const char * sGold = "gold";
	
};
#endif // __MYPLAYER_H__
