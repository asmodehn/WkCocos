#ifndef __PRELOAD_ENTITY_PRLEOAD_H__
#define __PRELOAD_ENTITY_PRELOAD_H__

#include "cocos2d.h"

#include "entityx/entityx.h"

#include "WkCocos/Download/Events/Downloaded.h"

#include "curl/curl.h"

namespace WkCocos 
{
	namespace Preload
	{
		class Preload : public entityx::Receiver<Preload>
			{
			public:
				explicit Preload(unsigned short concurrent_loads,
					std::function<void(float)> progress_callback/*,
					std::function<void(std::string)> error_callback*/
					);

				//scheduel the load in memory of a data.
				// passing it sdependencies prevent to load it if a dependency is missing.
				//DataLoad Event is sent when the load finishes.
				bool addDataLoad(const std::string &  filepath, const std::vector<std::string> & depends_filepath);
				
				virtual ~Preload();

				entityx::ptr<entityx::EventManager> getEventManager()
				{
					return event_manager;
				}

				entityx::ptr<entityx::SystemManager> getSystemManager()
				{
					return system_manager;
				}

				void setEventEmitter(entityx::ptr<entityx::EventManager> event_emitter);

				void receive(const Download::Events::Downloaded &dl);

				void update(double dt);

			protected:

				unsigned short m_concurrent_loads;

				//std::function<void(std::string)> m_error_callback;
				std::function<void(float)> m_progress_callback;

				entityx::ptr<entityx::EventManager> event_manager;
				entityx::ptr<entityx::EntityManager> entity_manager;
				entityx::ptr<entityx::SystemManager> system_manager;

			};

	} // namespace Preload
}  // namespace WkCocos

#endif //__PRELOAD_ENTITY_PRELOAD_H__
