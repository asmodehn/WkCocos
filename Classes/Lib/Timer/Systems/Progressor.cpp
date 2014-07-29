#include "WkCocos/Timer/Systems/Progressor.h"
#include "WkCocos/Timer/Comp/TimeValue.h"

namespace WkCocos
{
	namespace Timer
	{
		namespace Systems
		{
			/**
			* This function will call the callback with the current progress of the timer.
			*/
			void Progressor::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ID> id;
				entityx::ptr<Comp::Progress> prog;
				entityx::ptr<Comp::Callback> cb;
				for (auto entity : es->entities_with_components(id, prog, cb))
				{
					prog->m_current += static_cast<unsigned long>(dt * 1000 + 0.5);
					cb->m_cb(id->m_id, prog->m_current);
				}
			};

		}//namespace Systems
	}//namespace Timer
}//namespace WkCocos


