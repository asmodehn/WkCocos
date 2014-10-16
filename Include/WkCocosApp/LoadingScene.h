#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "WkCocos/Scene.h"

#include "MyPlayer.h"
#include "WkCocos/Download/Download.h"
#include "WkCocos/Preload/Preload.h"
#include "WkCocos/Download/Events/Error.h"
#include "WkCocos/Preload/Events/Error.h"

class LoadingScene : public WkCocos::Scene, public entityx::Receiver<LoadingScene>
{
public:
		
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LoadingScene);

	virtual ~LoadingScene();

	virtual void onEnterTransitionDidFinish() override;

	virtual void update(float delta) override;

	virtual void onExitTransitionDidStart() override;

	void addLoad(std::vector<std::string> respath);

	void setLoadDoneCallback(std::function<void()> cb);

	/**
	* schedules a DLC check to be done while loading.
	*
	*/
	void scheduleDLCCheck();

	void error_CB(std::string msg);

	//expects pct in [0..1]
	void progress_CB(float pct);

	void receive(const WkCocos::Player::Error &pe);
	void receive(const WkCocos::Download::Events::Error &de);
	void receive(const WkCocos::Preload::Events::Error &pe);
	void receive(const WkCocos::LocalData::Events::Error &ld);

protected:

	LoadingScene();

	bool m_loadDoneCB_called;

	bool m_loadMan_del_scheduled;
	WkCocos::Download::Download * m_downloadManager;
	WkCocos::Preload::Preload * m_preloadManager;
	std::function<void()> m_loadDoneCB;

};
#endif // __LOADING_SCENE_H__