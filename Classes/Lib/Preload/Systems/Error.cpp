#include "WkCocos/Preload/Systems/Error.h"
#include "WkCocos/Preload/Events/Error.h"

#include "cocos2d.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{

			Error::Error(std::function<void()> error_CB) : m_error_CB(error_CB)
			{}

			void Error::configure(entityx::ptr<entityx::EventManager> event_manager) {
				event_manager->subscribe<Events::Error>(*this);
			}

			void Error::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) {}

			void Error::receive(const Events::Error &err) {
				CCLOGERROR("Preload error");
				m_error_CB();
			}

					
		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

