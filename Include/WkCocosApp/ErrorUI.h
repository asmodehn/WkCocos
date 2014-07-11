#ifndef __ERRORUI_H__
#define __ERRORUI_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

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
	* Destructor
	*/
	virtual ~ErrorUI();

	bool * m_parentError;

	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void activate();
	void deactivate();

protected:
	cocos2d::ui::Button* m_refreshButton;

};

#endif // __ERRORUI_H__