#include "WkCocosApp/WebUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string WebUI::id = "webview";

WebUI::WebUI()
	: Interface()
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width / 2, visibleSize.height / 2));
		cocos2d::Size widgetSize = m_widget->getContentSize();

		webv = cocos2d::experimental::ui::WebView::create();
		webv->setContentSize(widgetSize);
		webv->loadFile("web_sample.html");
		webv->setScalesPageToFit(true);

		webv->setOnShouldStartLoading(std::bind(&WebUI::onWebViewShouldStartLoading, this, std::placeholders::_1, std::placeholders::_2));
		webv->setOnDidFinishLoading(std::bind(&WebUI::onWebViewDidFinishLoading, this, std::placeholders::_1, std::placeholders::_2));
		webv->setOnDidFailLoading(std::bind(&WebUI::onWebViewDidFailLoading, this, std::placeholders::_1, std::placeholders::_2));

		m_widget->addChild(webv);


		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}
}

WebUI::~WebUI()
{}

bool WebUI::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewShouldStartLoading, url is %s", url.c_str());

	return true;
}

void WebUI::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewDidFinishLoading, url is %s", url.c_str());

}

void WebUI::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewDidFailLoading, url is %s", url.c_str());

}

