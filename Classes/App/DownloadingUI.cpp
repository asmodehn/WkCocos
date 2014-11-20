#include "WkCocosApp/DownloadingUI.h"

#include "WkCocosApp/LoadingScene.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string DownloadingUI::id = "downloading";

DownloadingUI::DownloadingUI()
	: Interface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{
		//loadingbar
		auto loadingbar = cocos2d::ui::LoadingBar::create("loadingbar.png", 0.5f);
		loadingbar->setName("LoadingBar");
		m_widget->addChild(loadingbar);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}

}

DownloadingUI::~DownloadingUI()
{}
