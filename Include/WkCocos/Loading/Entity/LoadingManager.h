#ifndef __LOADING_ENTITY_LOADINGMANAGER_H__
#define __LOADING_ENTITY_LOADINGMANAGER_H__

#include "cocos2d.h"
#include "extensions/assets-manager/AssetsManager.h"

#include "entityx/entityx.h"
#include "generated/dataBase/DataResource.h"
#include "generated/dataBase/DataSlideshow.h"
#include "generated/dataBase/DataTextshow.h"

namespace dfgame 
{
	namespace Loading
	{
			class LoadingManager : public entityx::Manager
			{
			public:
				explicit LoadingManager(	int concurrent_loads,
											std::function<void(float)> progress_callback, 
											std::function<bool(DataResource::eResourceIds,double)> image_show_callback,
											std::function<bool(unsigned int,double)> text_show_callback,
											std::function<void()> error_callback
										)
					: m_concurrent_loads(concurrent_loads)
					, m_progress_callback(progress_callback)
					, m_imgshow_callback(image_show_callback)
					, m_txtshow_callback(text_show_callback)
					, m_error_callback(error_callback)
				{}

				//the assetsManager must be added to a scene to have its update called
				void addDataDownload(const std::string json_manifest_filename);

				void addDataResource(const std::vector<DataResource::eResourceIds> &  ids);

				double addDataSlideshow(DataSlideshow::eSlideshowIds id, double timer = 0);

				bool shiftSlideshowClock(double increment);

			protected:
				void configure() override;

				void initialize() override;

				void update(double dt) override;

				unsigned short m_concurrent_loads;
				std::function<void()> m_error_callback;
				std::function<void(float)> m_progress_callback;
				std::function<bool(DataResource::eResourceIds,double)> m_imgshow_callback;
				std::function<bool(unsigned int,double)> m_txtshow_callback;
				
			};

	} // namespace Loading
}  // namespace dfgame

#endif //__LOADING_ENTITY_LOADINGMANAGER_H__
