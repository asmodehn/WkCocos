#ifndef __ERRORUI_H__
#define __ERRORUI_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocosApp/LoadingScene.h"

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
	ErrorUI(LoadingScene* ls);

	/**
	* Destructor
	*/
	virtual ~ErrorUI();

	void setRefreshCallback(std::function<void()> cb);

	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void activate();
	void deactivate();

	std::function<void()> m_refreshCB;

	LoadingScene * parent;

protected:
	cocos2d::ui::Button* m_refreshButton;

};

#endif // __ERRORUI_H__