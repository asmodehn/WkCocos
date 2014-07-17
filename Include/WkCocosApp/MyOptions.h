#ifndef __MYOPTIONS_H__
#define __MYOPTIONS_H__

#include "WkCocos/Options.h"

/**
* This UI is the main menu UI
*/
class MyOptions : public WkCocos::Options
{
public:

	/**
	* Constructor
	*/
	MyOptions(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr);

	/**
	* Destructor
	*/
	virtual ~MyOptions();
	
};


#endif // __MYOPTIONS_H__
