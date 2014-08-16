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
	
private:

	const char * sCurrency = "currency";
	const char * sGem = "gem";
	const char * sGold = "gold";
	const char * sAlarms = "alarms";
	const char * sID = "id";
	//const char * sTime = "time";

	const char * sSec = "sec";
	const char * sMin = "min";
	const char * sHour = "hour";
	const char * sMday = "mday";
	const char * sMon = "mon";
	const char * sYear = "year";
	const char * sWday = "wday";
	const char * sYday = "yday";
	const char * sIsdst = "isdst";
	
};
#endif // __MYPLAYER_H__