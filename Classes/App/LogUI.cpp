#include "WkCocosApp/LogUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"
#include "WkCocos/Utils/log/logstream.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string LogUI::id = "log";

LogUI::LogUI()
	: Interface()
	, m_cocosAppnd(nullptr)
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		// Test for cocos log appender
		WkCocos::CocosLogAppender::CheckBoxRes resource;
		m_cocosAppnd = new WkCocos::CocosLogAppender(m_widget, resource);
		WkCocos::LogStream::get()->addAppender(m_cocosAppnd);
		LOG_INFO << "Add " << "cocos " << "appender" << std::endl;
		LOG_INFO << "Next Line" << std::endl;

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
}

LogUI::~LogUI()
{
	LOG_INFO << "Last Line" << std::endl;
	LOG_INFO << "Remove " << "cocos " << "appender" << std::endl;
	WkCocos::LogStream::get()->removeAppender(m_cocosAppnd);
	delete m_cocosAppnd;
}


