#ifndef __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__
#define __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__

#include "entityx/entityx.h"

#include "cocos/cocos2d.h"

namespace dfgame
{
	namespace Loading
	{
		
		namespace Comp
		{
			struct LoadSyncFunc : entityx::Component<LoadSyncFunc> {
				static const unsigned short load_priority_max = 100;
				static const unsigned short load_priority_min = 0;

				LoadSyncFunc(int apriority, std::function<void()> af) : loadfunction(af), priority(apriority) {}

				std::function<void()> loadfunction;
				//priority to determine the position in the queue of loading function
				unsigned short priority;
			};

			struct LoadASyncFunc : entityx::Component<LoadASyncFunc> {
				static const unsigned short load_priority_max = 100;
				static const unsigned short load_priority_min = 0;

				LoadASyncFunc(int apriority, std::function<void(std::function<void(cocos2d::Texture2D*)> endcb)> af) : loadfunction(af), priority(apriority), inprogress(false) {}

				std::function<void(std::function<void(cocos2d::Texture2D*)> endcb)> loadfunction;
				//priority to determine the position in the queue of loading function
				unsigned short priority;
				//marker to set while the loadfunction is running;
				bool inprogress;
			};

		}//namespace Comp
	}//namespace LoadingScene
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__
