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

				entityx::ptr<Comp::ServerTime> st;
				for (auto entity : entities->entities_with_components(st))
				{
					if (st->done)
					{
						CCLOG("Time Data entity lived %f seconds", st->life_time);
						entity.remove<Comp::ServerTime>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!st->in_progress)
					{
						m_timer_service->GetCurrentTimeNoWinBase(st->m_cb);
						st->in_progress = true;
					}
					else
					{
						st->life_time += dt;
						if (st->life_time > TIMEOUT)
						{
							st->timeout = true;
							events->emit<Events::Error>("server time");
						}
					}

				}

			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

