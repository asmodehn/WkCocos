#ifndef __LOGUI_H__
#define __LOGUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Utils/CocosAppender.h"

/**
* This UI is the main menu UI
*/
class LogUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	LogUI();

	/**
	* Destructor
	*/
	virtual ~LogUI();
	
protected:

	WkCocos::CocosLogAppender* m_cocosAppnd;

};


#endif // __LOGUI_H__
