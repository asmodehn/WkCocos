#ifndef __WKCOCOS_ACTOR_H__
#define __WKCOCOS_ACTOR_H__

#include "entityx/entityx.h"
#include "WkCocos/internal/Config.h"
#include "WkCocos/World.h"

namespace WkCocos
{
	/**
	 * This is the mother class of all WkCocos Actors.
	 */
	class Actor : public entityx::Receiver<Actor>
	{
	    //static World smart pointer to allow default constructor
	    static std::shared_ptr<World> s_default_world;

	protected:
		//the world this is a part of.
		std::shared_ptr<World> m_world;

		ActorID m_id;
	public:

		std::shared_ptr<entityx::EventManager> events();

		WKCOCOS_DEPRECATED_ATTRIBUTE static std::shared_ptr<entityx::EventManager> getEventManager();

        //constructor setting the world
		Actor(std::shared_ptr<World> world = s_default_world);

		//copy constructor
        Actor(const Actor& );

        //destructor
		virtual ~Actor();

        //Getting the ID of this Actor
		ActorID getId();

	};

}//namespace WkCocos

#endif // __WKCOCOS_ACTOR_H__
