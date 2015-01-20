#include "WkCocosApp/AdUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"
#include "WkCocos/Utils/log/logstream.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Utils/WkJniHelper.h"

const std::string AdUI::id = "ad";

AdUI::AdUI()
	: AppInterface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();


		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
}

AdUI::~AdUI()
{
}

void AdUI::onShow()
{
	CCLOG("onShow");
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	WkCocos::Utils::WkJniHelper::showAd(visibleSize / 2);
	CCLOG("onShow end");
}

void AdUI::onHide()
{
	CCLOG("onHide");
	WkCocos::Utils::WkJniHelper::hideAd();
	CCLOG("onHide end");
}

