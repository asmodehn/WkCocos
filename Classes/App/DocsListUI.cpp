#include "WkCocosApp/DocsListUI.h"

#include "WkCocosApp/GameLogic.h"
//#include "cocos2d.h"
//#include "cocos/ui/CocosGUI.h"

const std::string DocsListUI::id = "docs_list";

DocsListUI::DocsListUI()
	: Interface()
{
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

	}

	if (g_gameLogic->getPlayer().getOnlineDatamgr())
	{

	}

}

DocsListUI::~DocsListUI()
{

}