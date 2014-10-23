#include "WkCocos/Actor.h"

namespace WkCocos
{
	//static instances
	std::unordered_map<ActorID, Actor *> Actor::actors;

	std::default_random_engine Actor::generator;
	std::uniform_int_distribution<ActorID> Actor::distribution;
	
	std::shared_ptr<entityx::EventManager> Actor::event_manager;
	//

	Actor::Actor()
	{
		//creates event manager with first actor.
		if (!event_manager)
		{
			event_manager = entityx::EventManager::make();
		}

		//generate an id that is not in the list
		ActorID id = distribution(generator);  // generates id

		//tentative insert into actor map
		bool inserted = false;
		while (!inserted) // careful we have a max number of actors !!
		{
			std::pair<std::unordered_map<ActorID, Actor *>::iterator, bool> res = actors.insert(std::make_pair(id, this));
			if (res.second) {
				m_id = id;
			}
			else
			{
				id = distribution(generator);
			}
			inserted = res.second;
		}
	}
	
	Actor::~Actor()
	{
		//removing myself from actors list
		actors.erase(m_id);
	}

	ActorID Actor::getId()
	{
		return m_id;
	}


	/**
	* find actor in list from Id.
	* return nullptr if not found
	*/
	Actor* Actor::getActor(ActorID id)
	{
		std::unordered_map<ActorID, Actor*>::iterator it = actors.find(id);
		if (actors.end() != it)
		{//id found
			return it->second;
		}
		return nullptr;
	}
	
}//namespace WkCocos
