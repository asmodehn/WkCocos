#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "WkCocos/Scene.h"

#include "entityx/entityx.h"

#include "ui/CocosGUI.h"

#include "WkCocos/Download/Download.h"
#include "WkCocos/Download/Events/Error.h"

#include "WkCocos/Preload/Preload.h"
#include "WkCocos/Preload/Events/Error.h"

#include "WkCocos/OnlineData/OnlineDataManager.h"
#include "WkCocos/LocalData/LocalDataManager.h"
#include "MyPlayer.h"

class TestScene : public WkCocos::Scene, public entityx::Receiver<TestScene>
{
public:

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);

	virtual ~TestScene();

	virtual void onEnterTransitionDidFinish() override;

	virtual void update(float delta) override;

	virtual void onExitTransitionDidStart() override;

	void error_CB(std::string msg, std::function<void()> retryCB, std::function<void()> skipCB);

	void menuCloseCallback(cocos2d::Ref* pSender);

	void prevCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void nextCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

	void receive(const WkCocos::OnlineData::Events::Error &OD);
	void receive(const WkCocos::LocalData::Events::Error &LD);
	void receive(const MyPlayer::Error &PL);

	void receive(const WkCocos::Download::Events::Error &de);
	void receive(const WkCocos::Preload::Events::Error &pe);

protected:
	TestScene();

	WkCocos::Download::Download * m_downloadManager;
	WkCocos::Preload::Preload * m_preloadManager;

	std::string currentUI;

	cocos2d::ui::Text* m_time;

	cocos2d::ui::Button* m_nextButton;
	cocos2d::ui::Button* m_prevButton;
	cocos2d::ui::Text* m_nextLabel;
	cocos2d::ui::Text* m_prevLabel;
	cocos2d::ui::Text* m_titleLabel;

};

#endif // __TEST_SCENE_H__
