#ifndef __WKCOCOS_WORLD_H__
#define __WKCOCOS_WORLD_H__

#include "entityx/entityx.h"

#include <random>
#include <unordered_map>

namespace WkCocos
{

    class Actor; // forward definition

	typedef unsigned long ActorID;
    static const ActorID InvalidActorID = 0UL;

    /**
    * A basic event : allow communication between actors
    */
	template <typename Derived>
	class Event : public entityx::Event<Derived> {
		ActorID m_id;
	public:
		//forcing Actors to pass their ID in events they trigger
		Event(ActorID id)
			: m_id(id)
		{}

		ActorID getSenderID() const {
			return m_id;
		}
	};

    /**
    * World is an environment where all events are potentially visible to all actors
    * They just need to subscribe to it.
    */
    class World
    {
		//simple random number generators (no need for seed here)
		static std::default_random_engine generator;
		static std::uniform_int_distribution<ActorID> distribution;

   		std::unordered_map<ActorID, Actor *> actors;
		std::shared_ptr<entityx::EventManager> event_manager;

    public :
        /**
        * if UniqueWorld is true, all World instance will actually refer to the one and unique default world (Actor::s_default_world)
        * Their event manager will be the same. However the actor lists will be different.
        * This is needed to support old deprecated behavior ( one static world ).
        */
		World();

        World(const World& );

		~World();

		/**
		* Adds an Actor to this world
		* @return the Id of the actor
		*/
		ActorID addActor(Actor* actor);

        /**
		* find actor in list from Id.
		* @return nullptr if not found
		*/
		Actor* getActor(ActorID id);

        /**
		* Removes an Actor from this world
		* @return the Id of the actor
		*/
		void removeActor(ActorID id);

		std::shared_ptr<entityx::EventManager> getEventManager() {return event_manager;}
    };


} // namespace WkCocos

#endif //__WKCOCOS_WORLD_H__
