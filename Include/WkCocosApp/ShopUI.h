#ifndef __SHOPUI_H__
#define __SHOPUI_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class ShopUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	ShopUI();

	/**
	* Destructor
	*/
	virtual ~ShopUI();
	
	void update(float delta);

	void buyCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

protected:

	cocos2d::ui::Button* m_buyButton;
	cocos2d::ui::Text* m_buyLabel;
};


#endif // __SHOPUI_H__
