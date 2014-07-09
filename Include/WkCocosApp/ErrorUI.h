#ifndef __ERRORUI_H__
#define __ERRORUI_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class ErrorUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	ErrorUI(bool *parentError);

	/**
	* Constructor
	*/
	virtual ~ErrorUI();

	bool * m_parentError;

	void refreshCallback(cocos2d::Ref* pSender);

};

#endif // __ERRORUI_H__