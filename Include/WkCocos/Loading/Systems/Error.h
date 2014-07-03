#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_ERROR_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_ERROR_H__

#include "cocos2d.h"
#include "entityx/entityx.h"

#include "WkCocos/Loading/Events/Error.h"

namespace WkCocos 
{
	namespace Loading
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
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_ERROR_H__
