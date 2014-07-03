#ifndef __DFGAME_LOADINGSCENE_ENTITY_COMP_SLIDESHOW_H__
#define __DFGAME_LOADINGSCENE_ENTITY_COMP_SLIDESHOW_H__

#include "entityx/entityx.h"

#include "generated/dataBase/DataResource.h"

namespace dfgame
{
	namespace Loading
	{
		namespace Comp
		{
			/**
			* @param id is the resource id of the image to display
			* @param delay is the date when image must be displayed
			*/
			struct Image : entityx::Component<Image> {
				Image(DataResource::eResourceIds id, double showtime) : m_id(id),m_showtime(showtime) {}

				DataResource::eResourceIds m_id;
				double m_showtime;
			};

			struct Text : entityx::Component<Text> {
				/**
				* @param id is a loca id
				* @param delay is the date when text must be displayed
				*/
				Text(unsigned int id, double showtime) : m_id(id), m_showtime(showtime) {}

				unsigned int m_id;
				double m_showtime;
			};

			struct ShowProgress : entityx::Component<ShowProgress> {
				ShowProgress(double delay) : m_delay(delay) {}
				double m_delay;
			};

		}//namespace Comp
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADINGSCENE_ENTITY_COMP_SLIDESHOW_H__
