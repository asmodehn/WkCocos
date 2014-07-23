#ifndef __DFGAME_PRELOAD_ENTITY_SYSTEMS_ERROR_H__
#define __DFGAME_PRELOAD_ENTITY_SYSTEMS_ERROR_H__

#include "cocos2d.h"
#include "entityx/entityx.h"

#include "WkCocos/Preload/Events/Error.h"

namespace WkCocos 
{
	namespace Preload
	{
		namespace Systems
		{
			//system finding error and launching error behavior
			struct Error : public entityx::System<Error>, entityx::Receiver<Error>
			{

				Error(std::function<void()> error_CB);
					
				void configure(entityx::ptr<entityx::EventManager> event_manager) override;

				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;

				void receive(const Events::Error &err);

				std::function<void()> m_error_CB;
			};
		}//namespace Systems
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_SYSTEMS_ERROR_H__
