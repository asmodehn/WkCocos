#include "WkCocosApp/DownloadUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string DownloadUI::id = "download";

DownloadUI::DownloadUI()
	: Interface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height * 0.67f));
	cocos2d::Size widgetSize = m_widget->getContentSize();

	sprite = cocos2d::ui::ImageView::create("HelloWorld.png");
	m_widget->addChild(sprite);
	sprite->setScaleX(widgetSize.width / sprite->getContentSize().width);
	sprite->setScaleY(widgetSize.height / sprite->getContentSize().height);
	sprite->setPosition(cocos2d::Vec2(widgetSize.width * 0.5, widgetSize.height * 0.5));
	sprite->setVisible(true);

	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

DownloadUI::~DownloadUI()
{}

