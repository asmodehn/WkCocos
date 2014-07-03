#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DISPLAY_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DISPLAY_H__

#include "cocos2d.h"
#include "entityx/entityx.h"

#include "dfgame/Loading/Entity/Comp/Slideshow.h"

namespace dfgame
{
	namespace Loading
	{
		namespace Systems
		{
			//system finding display components and showing them
			struct Display : public entityx::System<Display>
			{
				Display(std::function<bool(DataResource::eResourceIds, double)> imagedisplay_cb, std::function<bool(unsigned int, double)> textdisplay_cb);

				double shiftClock(double increment);

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt);

			protected :
				std::function<bool(DataResource::eResourceIds, double)> m_imagedisplay_cb;
				std::function<bool(unsigned int, double)> m_textdisplay_cb;

				double m_clock;

			};
		}//namespace Systems
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_DISPLAY_H__
