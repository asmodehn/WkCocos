#include "WkCocos/World.h"
#include "WkCocos/Actor.h"
namespace WkCocos
{
    //static instances

	std::default_random_engine World::generator;
	std::uniform_int_distribution<ActorID> World::distribution;

    World::World()
    {
        event_manager = entityx::EventManager::make(); // we make a new separated event manager for this world
    }

    World::World(const World& w)
    {
        //NOT SURE WHICH BEHAVIOR IS BEST...
        event_manager = entityx::EventManager::make();
        //event_manager = w.event_manager;
    }

    World::~World()
    {
    }

    ActorID World::addActor(Actor* act)
	{
        //generate an id that is not in the list
        ActorID id = distribution(generator);  // generates id

        //tentative insert into actor map
        bool inserted = false;
        while (!inserted) // careful we have a max number of actors !!
        {
            std::pair<std::unordered_map<ActorID, Actor *>::iterator, bool> res = actors.insert(std::make_pair(id, act));
            if (!res.second)// cannot be inserted -> regenerate id
            {
                id = distribution(generator);
            }
            inserted = res.second;
            //TODO : make sure we never loop infinitely.
            //Emergency exit : return InvalidActorID;
        }
        return id;
	}

	Actor* World::getActor(ActorID id)
	{
        //removing myself from actors list if needed
        std::unordered_map<ActorID, Actor*>::iterator meit = actors.find(id);
        if (actors.end() != meit)
        {
            return meit->second;
        }
        else
        {
            return nullptr;
        }
	}

	void World::removeActor(ActorID id)
	{
        //removing myself from actors list if needed
        std::unordered_map<ActorID, Actor*>::iterator meit = actors.find(id);
        if (actors.end() != meit)
        {//id found
            actors.erase(meit);
        }
	}

}// namespace WkCocos
