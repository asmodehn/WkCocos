#ifndef __PRELOADUI_H__
#define __PRELOADUI_H__

#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Preload/Preload.h"

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

    /**
    * Getting the preload manager to listen to
    */
    WkCocos::Preload::Preload* getPreloadManager()
    {
        return m_plmgr;
    };

    /**
    * Callback called when clicking on DL button
    */
    void PLCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

    virtual void update(float delta) override;

protected:

	cocos2d::ui::ImageView* sprite;

    bool m_plStarted;
	WkCocos::Preload::Preload* m_plmgr;
};


#endif // __TIMERUI_H__
