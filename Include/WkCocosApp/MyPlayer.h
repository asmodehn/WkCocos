#ifndef __MYPLAYER_H__
#define __MYPLAYER_H__

#include "WkCocos/Player.h"

/**
* This is the game player
*/
class MyPlayer : public WkCocos::Player<MyPlayer>
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
	void get_data_json(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator) override;

	/**
	* data setter from json string
	*/
	void set_data_json(rapidjson::Document& doc) override;

	//ingame currency
	unsigned int m_gold;
	//premium currency
	unsigned int m_gem;

	//save Data for test
	void saveData()
	{
		requestSaveData();
	}

	//load Data for test
	void loadData()
	{
		requestLoadData();
	}
};


#endif // __MYPLAYER_H__
