#ifndef __PRELOADUI_H__
#define __PRELOADUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class PreloadUI : public WkCocos::Interface
{
public:

	static const std::string id;

	/**
	* Default constructor
	*/
	PreloadUI();

	/**
	* Destructor
	*/
	virtual ~PreloadUI();

protected:

	cocos2d::ui::ImageView* sprite;
};


#endif // __TIMERUI_H__
