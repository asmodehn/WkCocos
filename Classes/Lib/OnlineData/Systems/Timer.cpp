#include "WkCocos/OnlineData/Systems/Timer.h"
#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			Timer::Timer()
			{
				m_timer_service = ::App42::App42API::BuildTimerService();
			}

			Timer::~Timer()
			{
				//UserService::Terminate();
				//m_service = nullptr;
			}

			void Timer::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::ProgressUpdate> pu;

				entityx::ptr<Comp::ServerTime> st;
				for (auto entity : entities->entities_with_components(st, pu))
				{
					if (st->done)
					{
						CCLOG("Time Data entity lived %f seconds", pu->life_time);
						entity.remove<Comp::ServerTime>();
						entity.remove<Comp::ProgressUpdate>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!pu->in_progress)
					{
						m_timer_service->GetCurrentTimeNoWinBase(st->m_cb);
						pu->in_progress = true;
					}
					else
					{
						if (pu->life_time > TIMEOUT && !st->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							st->timeout = true;
							events->emit<Events::Error>(entity.id(), "server time");
						}
					}

				}

			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

