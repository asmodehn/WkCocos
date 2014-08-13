#include "WkCocos/Preload/Systems/Error.h"
#include "WkCocos/Preload/Events/Error.h"

#include "cocos2d.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Systems
		{

			Error::Error(std::function<void(std::string)> error_CB) : m_error_CB(error_CB)
			{}

			void Error::configure(entityx::ptr<entityx::EventManager> event_manager) {
				event_manager->subscribe<Events::Error>(*this);
			}

			void Error::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) {}

			void Error::receive(const Events::Error &err) {
				m_error_CB("Preload error");
			}

					
		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

