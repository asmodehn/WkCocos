#ifndef __LOADINGUI_H__
#define __LOADINGUI_H__

#include "cocostudio/CocoStudio.h"

#include "WkCocos/Interface.h"

#include "WkCocos/Download/Download.h"

/**
* This UI is the main menu UI
*/
class DownloadingUI : public WkCocos::Interface, public entityx::Receiver<DownloadingUI>
{
public:

	static const std::string id;

	/**
	* Default constructor
	*/
	DownloadingUI();

	/**
	* Destructor
	*/
	virtual ~DownloadingUI();

    /**
    * Getting the download manager to listen to
    */
    WkCocos::Download::Download* getDownloadManager()
    {
        return m_dlmgr;
    };

    /**
    * Callback called when clicking on DL button
    */
    void DLCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

    /**
    * Receiving DownloadOptions event
    */
	void receive(const WkCocos::Download::Events::DownloadOptions &de);

    /**
    * Receiving DownloadAdvised event
    */
	void receive(const WkCocos::Download::Events::DownloadAdvised &de);

    /**
    * update on Interface is called when scene update is called
    */
	void update(float delta) override;

private:

    bool m_dlStarted;
    WkCocos::Download::Download* m_dlmgr;
};


#endif // __LOADINGUI_H__
