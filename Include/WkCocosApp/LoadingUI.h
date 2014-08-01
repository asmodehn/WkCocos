#ifndef __LOADINGUI_H__
#define __LOADINGUI_H__

#include "cocostudio/CocoStudio.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class LoadingUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	LoadingUI();

	/**
	* Destructor
	*/
	virtual ~LoadingUI();
	
};


#endif // __LOADINGUI_H__
