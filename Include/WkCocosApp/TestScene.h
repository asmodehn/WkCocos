#ifndef __SAVING_SCENE_H__
#define __SAVING_SCENE_H__

#include "WkCocos/Scene.h"

class TestScene : public WkCocos::Scene
{
public:
		
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);

	virtual ~TestScene();

	virtual void onEnterTransitionDidFinish() override;

	virtual void update(float delta) override;

	virtual void onExitTransitionDidStart() override;

	
	void error_CB();


protected:

	TestScene();

};


#endif // __SAVING_SCENE_H__
