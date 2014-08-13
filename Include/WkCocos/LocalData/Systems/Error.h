#ifndef __DFGAME_LOCALDATA_SYSTEMS_ERROR_H__
#define __DFGAME_LOCALDATA_SYSTEMS_ERROR_H__

#include "entityx/entityx.h"

#include "WkCocos/LocalData/Events/Error.h"

namespace WkCocos 
{
	namespace LocalData
	{
		namespace Systems
		{
			//system finding error and launching error behavior
			struct Error : public entityx::System<Error>, entityx::Receiver<Error>
			{
				Error(std::function<void(std::string)> error_CB);
					
				void configure(entityx::ptr<entityx::EventManager> event_manager) override;

				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;

				void receive(const Events::Error &err);

				std::function<void(std::string)> m_error_CB;
			};
		}//namespace Systems
	}//namespace LocalData
}//namespace WkCocos

#endif // __DFGAME_LOCALDATA_SYSTEMS_ERROR_H__
