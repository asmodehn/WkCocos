#ifndef __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_ERROR_H__
#define __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_ERROR_H__

#include "cocos2d.h"
#include "entityx/entityx.h"

#include "WkCocos/Download/Events/Error.h"

namespace WkCocos 
{
	namespace Download
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
	}//namespace Download
}//namespace WkCocos

#endif // __DFGAME_DOWNLOAD_ENTITY_SYSTEMS_ERROR_H__
