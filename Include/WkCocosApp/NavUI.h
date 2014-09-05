#ifndef __NAVUI_H__
#define __NAVUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "entityx/entityx.h"

/**
* This UI is the main menu UI
*/
class NavUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	NavUI(entityx::ptr<entityx::EventManager> events);

	/**
	* Destructor
	*/
	virtual ~NavUI();

	//callbacks for cocos
	void nextCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void prevCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	//events triggered
	struct Next : public entityx::Event<Next>{};
	struct Prev : public entityx::Event<Prev>{};

	void setTitle(std::string);

protected:

	entityx::ptr<entityx::EventManager> event_manager;

	cocos2d::ui::Button* m_nextButton;
	cocos2d::ui::Button* m_prevButton;
	cocos2d::ui::Text* m_nextLabel;
	cocos2d::ui::Text* m_prevLabel;
	cocos2d::ui::Text* m_titleLabel;
};


#endif // __NAVUI_H__
