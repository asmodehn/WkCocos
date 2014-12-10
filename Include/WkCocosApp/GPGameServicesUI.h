#ifndef __GPGAMESERVICESUI_H__
#define __GPGAMESERVICESUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class GPGameServicesUI : public WkCocos::Interface
{
public:

	static const std::string id;

	/**
	* Default constructor
	*/
	GPGameServicesUI();

	/**
	* Destructor
	*/
	virtual ~GPGameServicesUI();

	void signInCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void signOutCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

protected:

	cocos2d::ui::Button* m_signInButton;
	cocos2d::ui::Button* m_signOutButton;
	cocos2d::ui::Text* m_signInLabel;
	cocos2d::ui::Text* m_signOutLabel;
};


#endif // __GPGAMESERVICESUI_H__
