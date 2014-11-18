#include "WkCocos/Actor.h"

namespace WkCocos
{
    std::shared_ptr<World> Actor::s_default_world = std::make_shared<World>();

	Actor::Actor( std::shared_ptr<World> world )
	: m_world(world)
	{
	    if ( m_world)
        {
            m_id = m_world->addActor(this);
        }
        else
        {
            throw std::logic_error("This Actor doesnt have a world!!! This should never happen.");
        }
	}

	Actor::Actor(const Actor& a)
	{
        m_world = a.m_world;
	    if ( m_world)
        {
            m_id = m_world->addActor(this);
        }
        else
        {
            throw std::logic_error("This Actor lost his world!!! This should never happen.");
        }
	}

	Actor::~Actor()
	{
	    if ( m_world)
        {
            m_world->removeActor(m_id);
        }
        else
        {
            throw std::logic_error("This Actor lost his world!!! This should never happen.");
        }
	}

	ActorID Actor::getId()
	{
		return m_id;
	}

	std::shared_ptr<entityx::EventManager> Actor::events()
	{
	    if ( m_world)
        {
            return m_world->getEventManager();
        }
        else
        {
            throw std::logic_error("This Actor lost his world!!! This should never happen.");
        }
	}


	std::shared_ptr<entityx::EventManager> Actor::getEventManager()
	{
	    if ( s_default_world)
        {
            return s_default_world->getEventManager();
        }
        else
        {
            throw std::logic_error("The default world was lost!!! This should never happen.");
        }
	}

}//namespace WkCocos
