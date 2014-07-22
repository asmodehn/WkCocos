#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "WkCocos/Scene.h"

#include "WkCocos/Loading/Download.h"
#include "WkCocos/Loading/Preload.h"

class LoadingScene : public WkCocos::Scene
{
public:
		
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LoadingScene);

	virtual ~LoadingScene()
	{}

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

	void error_CB()
	{
		CCLOGERROR("ERROR");
	}

	//expects pct in [0..1]
	void progress_CB(float pct);

protected:

	LoadingScene()
		: Scene()
		, m_loadDoneCB_called(false)
		, m_loadDoneCB()
		, m_downloadManager(5,
		std::bind(&LoadingScene::progress_CB, this, std::placeholders::_1),
		std::bind(&LoadingScene::error_CB, this))
		, m_preloadManager(1,
		std::bind(&LoadingScene::progress_CB, this, std::placeholders::_1),
		std::bind(&LoadingScene::error_CB, this))
	{
	}

	bool m_loadDoneCB_called;
	std::function<void()> m_loadDoneCB;

	WkCocos::Loading::Download m_downloadManager;
	WkCocos::Loading::Preload m_preloadManager;
};


#endif // __LOADING_SCENE_H__
