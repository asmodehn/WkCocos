#ifndef __SHOPUI_H__
#define __SHOPUI_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Shop/ShopAssets.h"

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

	void buyCallback(WkCocos::Shop::ShopAssets::VirtualCurrencyPack vcp, cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

protected:

	std::vector<cocos2d::ui::Text*> m_curLabel;
	std::vector<cocos2d::ui::Button*> m_buyCurButton;
	
	cocos2d::ui::Button* m_refreshButton;
	cocos2d::ui::Text* m_refreshLabel;
};


#endif // __SHOPUI_H__
