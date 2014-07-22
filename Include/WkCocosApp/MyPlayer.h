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
	std::string get_data_json();

	/**
	* data setter from json string
	*/
	void set_data_json(std::string data);

	//ingame currency
	unsigned int m_gold;
	//premium currency
	unsigned int m_gem;

	void testSave()
	{

		requestSaveData();
	}
};


#endif // __MYPLAYER_H__
