#ifndef __DOWNLOADUI_H__
#define __DOWNLOADUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class DownloadUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	DownloadUI();

	/**
	* Destructor
	*/
	virtual ~DownloadUI();
	
protected:

	cocos2d::ui::ImageView* sprite;
};


#endif // __TIMERUI_H__
