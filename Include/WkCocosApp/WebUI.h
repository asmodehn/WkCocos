#ifndef __WEBUI_H__
#define __WEBUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/WebView/UIWebView.h"

/**
* This UI is the main menu UI
*/
class WebUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	WebUI();

	/**
	* Destructor
	*/
	virtual ~WebUI();
	

	/**
	* hideMe
	*/
	void hideMe()
	{
		webv->setVisible(false);
	}

	/**
	* showMe
	*/
	void showMe()
	{
		webv->setVisible(true);
	}


	/**
	* callbacks
	*/
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);

	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);

	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);

protected:

	cocos2d::experimental::ui::WebView* webv;
};


#endif // __WEBUI_H__
