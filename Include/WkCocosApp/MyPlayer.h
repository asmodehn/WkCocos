#ifndef __MYPLAYER_H__
#define __MYPLAYER_H__

#include "WkCocos/Player.h"

/**
* This UI is the main menu UI
*/
class MyPlayer : public WkCocos::Player
{
public:

	/**
	* Constructor
	*/
	MyPlayer(std::string app_access_key, std::string app_secret_key);

	/**
	* Destructor
	*/
	virtual ~MyPlayer();
	
};


#endif // __MYPLAYER_H__
