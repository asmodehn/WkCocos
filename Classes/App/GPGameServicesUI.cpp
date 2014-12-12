#include "WkCocosApp/GPGameServicesUI.h"

#include "WkCocosApp/GPGSManager.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string GPGameServicesUI::id = "GooglePlay Game Services";

GPGameServicesUI::GPGameServicesUI()
	: Interface()
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
		m_signInButton->setPosition(cocos2d::Vec2(-widgetSize.width / 4, 0));
		m_widget->addChild(m_signInButton);

		m_signInLabel = cocos2d::ui::Text::create("SignIn", "Thonburi", 21);
		m_signInLabel->setPosition(m_signInButton->getPosition() + cocos2d::Vec2(0, m_signInButton->getContentSize().height));
		m_widget->addChild(m_signInLabel);

		m_signOutButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_signOutButton->addTouchEventListener(CC_CALLBACK_2(GPGameServicesUI::signOutCallback, this));
		m_signOutButton->setPosition(cocos2d::Vec2(widgetSize.width / 4, 0));
		m_widget->addChild(m_signOutButton);

		m_signOutLabel = cocos2d::ui::Text::create("SignOut", "Thonburi", 21);
		m_signOutLabel->setPosition(m_signOutButton->getPosition() + cocos2d::Vec2(0, m_signOutButton->getContentSize().height));
		m_widget->addChild(m_signOutLabel);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

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
        if ( ! GPGSManager::IsSignedIn() )
        {
            GPGSManager::BeginUserInitiatedSignIn();
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
        if ( GPGSManager::IsSignedIn() )
        {
            GPGSManager::SignOut();
        }
#endif
	}
}

