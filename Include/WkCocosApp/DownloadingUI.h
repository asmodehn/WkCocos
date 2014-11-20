#ifndef __LOADINGUI_H__
#define __LOADINGUI_H__

#include "cocostudio/CocoStudio.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class DownloadingUI : public WkCocos::Interface
{
public:

	static const std::string id;

	/**
	* Default constructor
	*/
	DownloadingUI();

	/**
	* Destructor
	*/
	virtual ~DownloadingUI();

};


#endif // __LOADINGUI_H__
