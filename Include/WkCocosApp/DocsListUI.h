#ifndef __DOCSLISTUI_H__
#define __DOCSLISTUI_H__

#include "cocos/ui/CocosGUI.h"
#include "WkCocos/Interface.h"
#include "WkCocos/OnlineData/OnlineDataManager.h"

/**
* This UI is the main menu UI
*/
class DocsListUI : public WkCocos::Interface, public entityx::Receiver<DocsListUI>
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	DocsListUI();

	/**
	* Destructor
	*/
	virtual ~DocsListUI();

	void refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void receive(const WkCocos::OnlineData::Events::DocsList &doclist);

protected:

	cocos2d::ui::Text* m_refreshLabel;
	cocos2d::Size m_widgetSize;

	std::vector<std::map<std::string, cocos2d::ui::Text*>> m_table;

	int m_quantity = 5;
	int m_offset = 0;
	int m_pages = 0;

	bool m_user_request;

};

#endif // __DOCSLISTUI_H__