#ifndef __WKCOCOS_ACTOR_H__
#define __WKCOCOS_ACTOR_H__

#include "entityx/entityx.h"

#include <random>
#include <unordered_map>

namespace WkCocos
{
	/**
	 * This is the mother class of all WkCocos Actors.
	 */
	typedef unsigned long ActorID;

	template <typename Derived>
	class Event : public entityx::Event<Derived> {
		ActorID m_id;
	public:
		//forcing Actors to pass their ID in events they trigger
		Event(ActorID id)
			: m_id(id)
		{}

		ActorID getSenderID() {
			return m_id;
		}
	};

	class Actor : public entityx::Receiver<Actor>
	{
		static std::unordered_map<ActorID, Actor *> actors;
		//simple random number generators (no need for seed here)
		static std::default_random_engine generator;
		static std::uniform_int_distribution<ActorID> distribution;

		static ActorID registerMe(Actor*);

	protected:
		//event manager as sharedptr in case we need to use it in a non-actor.
		static std::shared_ptr<entityx::EventManager> event_manager;
		ActorID m_id;
	public:

		static inline entityx::ptr<entityx::EventManager> getEventManager(){
			return event_manager;
		}

        //default constructor
		Actor();

		//copy constructor
        Actor(const Actor& );

        //destructor
		~Actor();

		ActorID getId();

		/**
		* find actor in list from Id.
		* return nullptr if not found
		*/
		static Actor* getActor(ActorID id);

	};

}//namespace WkCocos

#endif // __WKCOCOS_ACTOR_H__
