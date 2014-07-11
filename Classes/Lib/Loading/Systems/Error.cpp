#include "WkCocos/Loading/Systems/Error.h"
#include "WkCocos/Loading/Events/Error.h"

namespace WkCocos
{
	namespace Loading
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
				m_error_CB();
			}

					
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

