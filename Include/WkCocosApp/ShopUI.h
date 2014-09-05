#ifndef __SHOPUI_H__
#define __SHOPUI_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Shop/Assets.h"
#include "WkCocos/Shop/Shop.h"

/**
* This UI is the main menu UI
*/
class ShopUI : public WkCocos::Interface, public entityx::Receiver<ShopUI>
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

	void buyCallback(WkCocos::Shop::Assets::VirtualCurrencyPack vcp, cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void receive(const WkCocos::Shop::Shop::CurrencyBalanceChanged& cbc);
	void receive(const WkCocos::Shop::Shop::MarketItemsRefreshed& cbc);

protected:

	std::map<std::string, cocos2d::ui::Text*> m_curMainLabel;
	std::map<std::string, cocos2d::ui::Text*> m_curPackLabel;
	std::map<std::string, cocos2d::ui::Text*> m_curPriceLabel;
	std::map<std::string, cocos2d::ui::Button*> m_buyCurButton;
	
	cocos2d::ui::Button* m_refreshButton;
	cocos2d::ui::Text* m_refreshLabel;
};


#endif // __SHOPUI_H__
