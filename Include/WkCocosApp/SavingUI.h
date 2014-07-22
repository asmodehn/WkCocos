#ifndef __SAVINGUI_H__
#define __SAVINGUI_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class SavingUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	SavingUI();

	/**
	* Destructor
	*/
	virtual ~SavingUI();
	

	void saveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

protected:

	cocos2d::ui::Button* m_saveButton;
};


#endif // __SAVINGUI_H__
