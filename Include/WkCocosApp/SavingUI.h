#ifndef __SAVINGUI_H__
#define __SAVINGUI_H__

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
	
	void update(float delta);

	void saveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void loadCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void gemrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void goldrandCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

protected:

	cocos2d::ui::Button* m_loadButton;
	cocos2d::ui::Button* m_saveButton;
	cocos2d::ui::Text* m_loadLabel;
	cocos2d::ui::Text* m_saveLabel;
	cocos2d::ui::Text* m_gemLabel;
	cocos2d::ui::Text* m_goldLabel;
	cocos2d::ui::Button* m_gemrandButton;
	cocos2d::ui::Button* m_goldrandButton;
};


#endif // __SAVINGUI_H__
