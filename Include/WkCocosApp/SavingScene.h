#ifndef __SAVING_SCENE_H__
#define __SAVING_SCENE_H__

#include "cocos2d.h"
#include "WkCocos/Scene.h"


class SavingScene : public WkCocos::Scene
{
public:
		
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SavingScene);

	virtual ~SavingScene();

	virtual void onEnterTransitionDidFinish() override;

	virtual void update(float delta) override;

	virtual void onExitTransitionDidStart() override;

	
	void error_CB();


protected:

	SavingScene();

};


#endif // __SAVING_SCENE_H__
