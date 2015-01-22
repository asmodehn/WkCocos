#ifndef __GPGAMESERVICESUI_H__
#define __GPGAMESERVICESUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Utils/GPGSManager.h"

#define ACHIEVEMENT1_ID "CgkI6PqAkZcJEAIQAw"
#define ACHIEVEMENT2_ID "CgkI6PqAkZcJEAIQBA"
#define ACHIEVEMENT3_ID "CgkI6PqAkZcJEAIQBQ"
#define ACHIEVEMENT4_ID "CgkI6PqAkZcJEAIQBg"
#define ACHIEVEMENT5_ID "CgkI6PqAkZcJEAIQBw"

/**
* This UI is the main menu UI
*/
class GPGameServicesUI : public WkCocos::Interface, public entityx::Receiver<GPGameServicesUI>
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

    void ach1Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
    void ach2Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
    void ach3Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
    void ach4Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
    void ach5Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

    void receive(const GPGSManager::SignedIn&);
    void receive(const GPGSManager::SignedOut&);

protected:

    GPGSManager* gpgs;

	cocos2d::ui::Button* m_signInButton;
	cocos2d::ui::Button* m_signOutButton;
	cocos2d::ui::Text* m_signInLabel;
	cocos2d::ui::Text* m_signOutLabel;

	cocos2d::ui::Button* m_ach1Button;
	cocos2d::ui::Text* m_ach1Label;
	cocos2d::ui::Button* m_ach2Button;
	cocos2d::ui::Text* m_ach2Label;
	cocos2d::ui::Button* m_ach3Button;
	cocos2d::ui::Text* m_ach3Label;
	cocos2d::ui::Button* m_ach4Button;
	cocos2d::ui::Text* m_ach4Label;
	cocos2d::ui::Button* m_ach5Button;
	cocos2d::ui::Text* m_ach5Label;
};


#endif // __GPGAMESERVICESUI_H__
