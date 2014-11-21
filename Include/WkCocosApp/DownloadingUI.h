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
    * Setting the download manager to listen to
    */
    void setDownloadManager( WkCocos::Download::Download* );

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
	* Called from the Scene ( since the scene setup the download manager )
	* expects pct in [0..1]
	*/
	void progress_CB(float pct);

    /**
    * update on Interface is called when scene update is called
    */
	void update(float delta) override;

private:

    bool m_dlStarted;
    WkCocos::Download::Download* m_dlmgr;
};


#endif // __LOADINGUI_H__
