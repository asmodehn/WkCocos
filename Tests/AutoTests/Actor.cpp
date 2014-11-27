// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/Actor.h"

#include <string>

SUITE(Actor)
{
    struct TestEvent : WkCocos::Event<TestEvent>
    {
            TestEvent(WkCocos::ActorID id) : WkCocos::Event<TestEvent>(id) {}
    };

    //One Actor, Default World, events on itself. => One actor should be able to listen his own events.
	struct UniqueImplicitWorld_Same : public WkCocos::Actor
	{
		UniqueImplicitWorld_Same() : WkCocos::Actor(), m_event_received(false) { /* some setup */ }
		~UniqueImplicitWorld_Same() { /* some teardown */ }

		bool m_event_received;
		void receive(const TestEvent&)
		{
		    m_event_received = true;
		}
	};

	//Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Same, UniqueImplicitWorld_Same_static_deprecated_getEventManager_getEventManager)
	{
	    UniqueImplicitWorld_Same::getEventManager()->subscribe<TestEvent>(*this);

	    UniqueImplicitWorld_Same::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}

    //Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Same, UniqueImplicitWorld_Same_static_deprecated_getEventManager_events)
	{
	    UniqueImplicitWorld_Same::getEventManager()->subscribe<TestEvent>(*this);

	    events()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}

    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Same, UniqueImplicitWorld_Same_events_getEventManager)
	{
	    events()->subscribe<TestEvent>(*this);

	    UniqueImplicitWorld_Same::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}

    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Same, UniqueImplicitWorld_Same_events_events)
	{
	    events()->subscribe<TestEvent>(*this);

	    events()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}



    //Two Actor, Default World, events from outside. => By default, actors must [be part of/share] the same world.
	struct UniqueImplicitWorld_Diff : public WkCocos::Actor
	{
		UniqueImplicitWorld_Diff() : WkCocos::Actor(), m_actor1(), m_event_received(false) { /* some setup */ }
		~UniqueImplicitWorld_Diff() { /* some teardown */ }

		WkCocos::Actor m_actor1;

		bool m_event_received;
		void receive(const TestEvent&)
		{
		    m_event_received = true;
		}
	};

	//Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Diff, UniqueImplicitWorld_Diff_getEventManager_getEventManager)
	{
	    UniqueImplicitWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    UniqueImplicitWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
    }

	//Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Diff, UniqueImplicitWorld_Diff_getEventManager_events)
	{
	    UniqueImplicitWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK ( m_event_received );
	}

    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Diff, UniqueImplicitWorld_Diff_events_getEventManager)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    UniqueImplicitWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}

	    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(UniqueImplicitWorld_Diff, UniqueImplicitWorld_Diff_events_events)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}


    //Two Actor, One World, events from outside. => Actor sharing the same world, explicitely.
	struct OneWorld_Diff : public WkCocos::Actor
	{
		OneWorld_Diff() : WkCocos::Actor(), m_actor1(this->m_world), m_event_received(false) { /* some setup */ }
		~OneWorld_Diff() { /* some teardown */ }

		WkCocos::Actor m_actor1;

		bool m_event_received;
		void receive(const TestEvent&)
		{
		    m_event_received = true;
		}
	};

	//Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(OneWorld_Diff, OneWorld_Diff_getEventManager_getEventManager)
	{
	    OneWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    OneWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
    }

	//Testing if old deprecated static Event Manager works as default unique world ( for actors without world specified )
	TEST_FIXTURE(OneWorld_Diff, OneWorld_Diff_getEventManager_events)
	{
	    OneWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK ( m_event_received );
	}


    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(OneWorld_Diff, OneWorld_Diff_events_getEventManager)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    OneWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}

    //Testing if new per actor events() works as default unique world ( for actors without world specified )
	TEST_FIXTURE(OneWorld_Diff, OneWorld_Diff_events_events)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK( m_event_received );
	}



    //Two Actor, Two World, events from outside. => Actor not sharing the same world.
	struct TwoWorld_Diff : public WkCocos::Actor
	{
		TwoWorld_Diff() : WkCocos::Actor(), m_world1(new WkCocos::World()), m_actor1(m_world1), m_event_received(false) { /* some setup */ }
		~TwoWorld_Diff() { /* some teardown */ }

        std::shared_ptr<WkCocos::World> m_world1;
		WkCocos::Actor m_actor1;

		bool m_event_received;
		void receive(const TestEvent&)
		{
		    m_event_received = true;
		}
	};

	//Testing if old deprecated static Event Manager works
	TEST_FIXTURE(TwoWorld_Diff, TwoWorld_Diff_getEventManager_getEventManager)
	{
	    TwoWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    TwoWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( m_event_received ); //same world -> we get the event
    }

	//Testing if old deprecated static Event Manager works
	TEST_FIXTURE(TwoWorld_Diff, TwoWorld_Diff_getEventManager_events)
	{
	    TwoWorld_Diff::getEventManager()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK ( ! m_event_received );// different world -> we dont get the event
	}


    //Testing if new per actor events() works
	TEST_FIXTURE(TwoWorld_Diff, TwoWorld_Diff_events_getEventManager)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    TwoWorld_Diff::getEventManager()->emit<TestEvent>(getId());

	    CHECK( ! m_event_received ); // different world -> we dont get the event
	}

    //Testing if new per actor events() works
	TEST_FIXTURE(TwoWorld_Diff, TwoWorld_Diff_events_events)
	{
	    m_actor1.events()->subscribe<TestEvent>(*this);

	    m_actor1.events()->emit<TestEvent>(getId());

	    CHECK( m_event_received ); //same world -> we get the event
	}



}
