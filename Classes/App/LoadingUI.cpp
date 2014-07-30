#include "WkCocosApp/LoadingUI.h"

#include "WkCocosApp/LoadingScene.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string LoadingUI::id = "loading";

LoadingUI::LoadingUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();
	//loadingbar
	auto loadingbar = cocos2d::ui::LoadingBar::create("loadingbar.png", .5f);
	loadingbar->setName("LoadingBar");

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	// position the label on the center of the screen
	loadingbar->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2,
		origin.y + loadingbar->getContentSize().height + 20 /* magic size margin */));


	m_widget->addChild(loadingbar);
		
	if (m_widget)
	{
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

LoadingUI::~LoadingUI()
{}