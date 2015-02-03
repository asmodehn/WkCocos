#include "WkCocosApp/GPGameServicesUI.h"

#include "WkCocos/Utils/GPGSManager.h"
#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string GPGameServicesUI::id = "GooglePlay Game Services";

GPGameServicesUI::GPGameServicesUI()
	: Interface()
	, gpgs(nullptr)
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		m_signInButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_signInButton->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::signInCallback, this));
		m_signInButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, widgetSize.height / 6));
		m_widget->addChild(m_signInButton);

		m_signInLabel = cocos2d::ui::Text::create("SignIn", "fonts/Marker Felt.ttf", 21);
		m_signInLabel->setPosition(m_signInButton->getPosition() + cocos2d::Vec2(0, m_signInButton->getContentSize().height));
		m_widget->addChild(m_signInLabel);

		m_signOutButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_signOutButton->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::signOutCallback, this));
		m_signOutButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, widgetSize.height / 6));
		m_widget->addChild(m_signOutButton);

		m_signOutLabel = cocos2d::ui::Text::create("SignOut", "fonts/Marker Felt.ttf", 21);
		m_signOutLabel->setPosition(m_signOutButton->getPosition() + cocos2d::Vec2(0, m_signOutButton->getContentSize().height));
		m_widget->addChild(m_signOutLabel);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

        gpgs = GPGSManager::getInstance();
        if (gpgs)
        {
            CCLOGERROR("SUBSCRIBING");
            gpgs->getEventManager()->subscribe<GPGSManager::SignedIn>(*this);
            gpgs->getEventManager()->subscribe<GPGSManager::SignedOut>(*this);
            gpgs->getEventManager()->subscribe<GPGSManager::SnapshotSaveRequested>(*this);
            //subscribing the player so he knows what to load and when.
            gpgs->getEventManager()->subscribe<GPGSManager::SnapshotLoaded>(g_gameLogic->getPlayer());
        }
	}
}

GPGameServicesUI::~GPGameServicesUI()
{}

void GPGameServicesUI::signInCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("SIGNIN BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if ( gpgs && ! gpgs->IsSignedIn() )
        {
            gpgs->BeginUserInitiatedSignIn();
        }
#endif
	}
}


void GPGameServicesUI::signOutCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("SIGNOUT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if ( gpgs && gpgs->IsSignedIn() )
        {
            gpgs->SignOut();
        }
#endif
	}
}

void GPGameServicesUI::receive(const GPGSManager::SignedIn&)
{
    CCLOGERROR("SIGNEDIN EVENT RECEIVED");

    cocos2d::Size widgetSize = m_widget->getContentSize();

    //ACH1
    m_ach1Button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_ach1Button->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::ach1Unlock, this));
    m_ach1Button->setPosition(cocos2d::Vec2(widgetSize.width * (-4) / 10, - widgetSize.height / 6));
    m_widget->addChild(m_ach1Button);

    m_ach1Label = cocos2d::ui::Text::create("Ach1", "fonts/Marker Felt.ttf", 21);
    m_ach1Label->setPosition(m_ach1Button->getPosition() + cocos2d::Vec2(0, m_ach1Button->getContentSize().height));
    m_widget->addChild(m_ach1Label);

    //ACH2
    m_ach2Button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_ach2Button->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::ach2Unlock, this));
    m_ach2Button->setPosition(cocos2d::Vec2(widgetSize.width * (-2) / 10, - widgetSize.height / 6));
    m_widget->addChild(m_ach2Button);

    m_ach2Label = cocos2d::ui::Text::create("Ach2", "fonts/Marker Felt.ttf", 21);
    m_ach2Label->setPosition(m_ach2Button->getPosition() + cocos2d::Vec2(0, m_ach2Button->getContentSize().height));
    m_widget->addChild(m_ach2Label);

    //ACH3
    m_ach3Button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_ach3Button->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::ach3Unlock, this));
    m_ach3Button->setPosition(cocos2d::Vec2(widgetSize.width * (0), - widgetSize.height / 6));
    m_widget->addChild(m_ach3Button);

    m_ach3Label = cocos2d::ui::Text::create("Ach3", "fonts/Marker Felt.ttf", 21);
    m_ach3Label->setPosition(m_ach3Button->getPosition() + cocos2d::Vec2(0, m_ach3Button->getContentSize().height));
    m_widget->addChild(m_ach3Label);

    //ACH4
    m_ach4Button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_ach4Button->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::ach4Unlock, this));
    m_ach4Button->setPosition(cocos2d::Vec2(widgetSize.width * (2) / 10, - widgetSize.height / 6));
    m_widget->addChild(m_ach4Button);

    m_ach4Label = cocos2d::ui::Text::create("Ach4", "fonts/Marker Felt.ttf", 21);
    m_ach4Label->setPosition(m_ach4Button->getPosition() + cocos2d::Vec2(0, m_ach4Button->getContentSize().height));
    m_widget->addChild(m_ach4Label);

    //ACH5
    m_ach5Button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_ach5Button->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::ach5Unlock, this));
    m_ach5Button->setPosition(cocos2d::Vec2(widgetSize.width * (4) / 10, - widgetSize.height / 6));
    m_widget->addChild(m_ach5Button);

    m_ach5Label = cocos2d::ui::Text::create("Ach5", "fonts/Marker Felt.ttf", 21);
    m_ach5Label->setPosition(m_ach5Button->getPosition() + cocos2d::Vec2(0, m_ach5Button->getContentSize().height));
    m_widget->addChild(m_ach5Label);

    //SAVE
    m_SaveButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_SaveButton->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::autosaveCallback, this));
    m_SaveButton->setPosition(cocos2d::Vec2(widgetSize.width * (3) / 10, - widgetSize.height *3/ 6));
    m_widget->addChild(m_SaveButton);

    m_SaveLabel = cocos2d::ui::Text::create("AutoSave", "fonts/Marker Felt.ttf", 21);
    m_SaveLabel->setPosition(m_SaveButton->getPosition() + cocos2d::Vec2(0, m_SaveButton->getContentSize().height));
    m_widget->addChild(m_SaveLabel);

    //SELECT
    m_SelectButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
    m_SelectButton->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::selectCallback, this));
    m_SelectButton->setPosition(cocos2d::Vec2(widgetSize.width * (0) / 10, - widgetSize.height *3/ 6));
    m_widget->addChild(m_SelectButton);

    m_SelectLabel = cocos2d::ui::Text::create("Select", "fonts/Marker Felt.ttf", 21);
    m_SelectLabel->setPosition(m_SelectButton->getPosition() + cocos2d::Vec2(0, m_SelectButton->getContentSize().height));
    m_widget->addChild(m_SelectLabel);

}

void GPGameServicesUI::receive(const GPGSManager::SignedOut&)
{
    CCLOGERROR("SIGNEDOUT EVENT RECEIVED");

    m_widget->removeChild(m_SaveLabel);
    m_widget->removeChild(m_SaveButton);

    m_widget->removeChild(m_SelectLabel);
    m_widget->removeChild(m_SelectButton);

    m_widget->removeChild(m_ach5Label);
    m_widget->removeChild(m_ach5Button);

    m_widget->removeChild(m_ach4Label);
    m_widget->removeChild(m_ach4Button);

    m_widget->removeChild(m_ach3Label);
    m_widget->removeChild(m_ach3Button);

    m_widget->removeChild(m_ach2Label);
    m_widget->removeChild(m_ach2Button);

    m_widget->removeChild(m_ach1Label);
    m_widget->removeChild(m_ach1Button);
}

void GPGameServicesUI::receive(const GPGSManager::SnapshotSaveRequested&)
{
    CCLOG("Save Requested");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
    {
        g_gameLogic->getPlayer().saveData(true);
    }
#endif
}

void GPGameServicesUI::ach1Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("ACHIEVEMENT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->unlockAchievement(ACHIEVEMENT1_ID);
        }
#endif
	}
}


void GPGameServicesUI::ach2Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("ACHIEVEMENT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->unlockAchievement(ACHIEVEMENT2_ID);
        }
#endif
	}
}


void GPGameServicesUI::ach3Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("ACHIEVEMENT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->unlockAchievement(ACHIEVEMENT3_ID);
        }
#endif
	}
}


void GPGameServicesUI::ach4Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("ACHIEVEMENT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->unlockAchievement(ACHIEVEMENT4_ID);
        }
#endif
	}
}


void GPGameServicesUI::ach5Unlock(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("ACHIEVEMENT BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->unlockAchievement(ACHIEVEMENT5_ID);
        }
#endif
	}
}

void GPGameServicesUI::autosaveCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("Save BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            g_gameLogic->getPlayer().saveData(true);
        }
#endif
	}
}

void GPGameServicesUI::selectCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("Select BUTTON CLICKED");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (gpgs && gpgs->IsSignedIn() )
        {
            gpgs->selectSnapshot("Google Snapshot Test",8);
        }
#endif
	}
}
