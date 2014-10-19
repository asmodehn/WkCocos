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

	if (m_widget)
	{
		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		sprite = cocos2d::ui::ImageView::create("HelloWorld.png");
		sprite->setScaleX(widgetSize.width / sprite->getContentSize().width);
		sprite->setScaleY(widgetSize.height / sprite->getContentSize().height);
		sprite->setVisible(true);
		m_widget->addChild(sprite);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}
}

DownloadUI::~DownloadUI()
{}