#ifndef __PRELOAD_ENTITY_PRLEOAD_H__
#define __PRELOAD_ENTITY_PRELOAD_H__

#include "cocos2d.h"

#include "entityx/entityx.h"

#include "curl/curl.h"

namespace WkCocos 
{
	namespace Preload
	{
		class Preload : public entityx::Manager
			{
			public:
				explicit Preload(unsigned short concurrent_loads,
					std::function<void(float)> progress_callback,
					std::function<void()> error_callback
					);

				//scheduel the load in memory of a data.
				//DataLoad Event is sent when the load finishes.
				bool addDataLoad(const std::vector<std::string> &  filepath);
				
				virtual ~Preload();

				entityx::ptr<entityx::EventManager> getEventManager()
				{
					return event_manager;
				}

				void setEventEmmiter(entityx::ptr<entityx::EventManager> event_emmiter);

			protected:
				void configure() override;

				void initialize() override;

				void update(double dt) override;

				unsigned short m_concurrent_loads;

				std::function<void()> m_error_callback;
				std::function<void(float)> m_progress_callback;

			};

	} // namespace Preload
}  // namespace WkCocos

#endif //__PRELOAD_ENTITY_PRELOAD_H__
