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
					if (!pu->in_progress)
					{
						m_timer_service->GetCurrentTimeNoWinBase([entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::ServerTime> ecst = entity.component<Comp::ServerTime>();
									if (ecst && ecst->m_cb) ecst->m_cb(data);
								}
								//job is done
								CCLOG("Get Server Time entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "server time");
						entity.destroy();
					}

				}

			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

