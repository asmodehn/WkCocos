#ifndef __MYPLAYER_H__
#define __MYPLAYER_H__

#include "WkCocos/Player.h"
#include "WkCocos/StrongBox/StrongBox.h"

/**
* This is the game player
*/
class MyPlayer : public WkCocos::Player
{
public:

	/**
	* Constructor
	*/
	MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr);

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
		requestSaveData(saved_cb, "53cr3t");
	}

	//load Data for test
	void loadData(std::function<void()> loaded_cb)
	{
		requestLoadData(loaded_cb, "53cr3t");
	}

	//load enemy for test
	void loadEnemy(std::string enemy_name)
	{
		requestEnemyData(enemy_name);
	}
	
	//test for getting every app user
	void getAllUsers()
	{
		requestAllUsers();
	}

	//test for getting users with at least one saved doc
	void getUsersWithDocs()
	{
		requestUsersWithDocs();
	}

private:

	const char * sCurrency = "currency";
	const char * sGem = "gem";
	const char * sGold = "gold";
	
};
#endif // __MYPLAYER_H__
