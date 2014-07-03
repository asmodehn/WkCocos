#ifndef __DFGAME_LOADING_ENTITY_COMP_IMAGE_H__
#define __DFGAME_LOADING_ENTITY_COMP_IMAGE_H__

#include "entityx/entityx.h"
#include "generated/dataBase/DataResource.h"

namespace dfgame
{
	namespace Loading
	{
		namespace Entity
		{
			namespace Comp
			{
				/**
				* @param id is the resource id of the image to display
				* @param delay is the date when image must be displayed
				*/
				struct Image : entityx::Component<Image> {
					Image(DataResource::eResourceIds id, double delay) : m_id(id), m_delay(delay), m_displayed(false) {}

					DataResource::eResourceIds m_id;
					double m_delay;
					bool m_displayed;
				};

			}//namespace Comp
		}//namespace Entity
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_COMP_IMAGE_H__
