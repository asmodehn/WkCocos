#include "WkCocos/OnlineData/Systems/ProgressUpdate.h"
#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			ProgressUpdate::ProgressUpdate()
			{
			}

			ProgressUpdate::~ProgressUpdate()
			{
			}

			void ProgressUpdate::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt)
			{

				entityx::ptr<Comp::ProgressUpdate> pu;
				for (auto entity : entities->entities_with_components(pu))
				{
					if (pu->in_progress)
						pu->life_time += dt;
				}

			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos