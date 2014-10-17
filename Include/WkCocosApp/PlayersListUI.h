#ifndef __PLAYERSLISTUI_H__
#define __PLAYERSLISTUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

/**
* This UI is the main menu UI
*/
class PlayersListUI : public WkCocos::Interface, public entityx::Receiver<PlayersListUI>
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	PlayersListUI();

	/**
	* Destructor
	*/
	virtual ~PlayersListUI();
	
	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void receive(const WkCocos::OnlineData::Events::PlayersList &pl);

protected:

	cocos2d::ui::Button* m_refreshButton;
	cocos2d::ui::Text* m_refreshLabel;
	cocos2d::ui::Text* m_enemyLabel;
	cocos2d::ui::Text* m_enemyData;
	cocos2d::Size m_widgetSize;

	std::map<std::string, cocos2d::ui::Text*> m_ptb;

	int quantity = 2;
	int offset = 0;

};

#endif // __PLAYERSLISTUI_H__