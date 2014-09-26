#ifndef __SAVING_SCENE_H__
#define __SAVING_SCENE_H__

#include "WkCocos/Scene.h"

#include "entityx/entityx.h"

#include "WkCocosApp/NavUI.h"

#include "WkCocos/OnlineData/OnlineDataManager.h"

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

	void error_CB(std::string msg);

	void menuCloseCallback(cocos2d::Ref* pSender);

	void receive(const NavUI::Next &dl);
	void receive(const NavUI::Prev &dl);

	//void receive(const WkCocos::OnlineData::Events::ServerTime &st);

protected:

	TestScene();

	//Event Manager passed to the UIs so they can emit and subscribe to events
	entityx::ptr<entityx::EventManager> ui_event_manager;

	std::string currentUI;

	cocos2d::ui::Text* m_time;

	bool m_waiting_for_server_time = false;
};


#endif // __SAVING_SCENE_H__
