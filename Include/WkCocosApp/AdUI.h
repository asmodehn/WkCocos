#ifndef __ADUI_H__
#define __ADUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocosApp/AppInterface.h"

/**
* This UI is the main menu UI
*/
class AdUI : public AppInterface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	AdUI();

	/**
	* Destructor
	*/
	virtual ~AdUI();

	/**
	* Event show
	*/
	virtual void onShow() override;

	/**
	* Event hide
	*/
	virtual void onHide() override;

	void showInterstitialAd(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
protected:


};


#endif // __ADUI_H__
