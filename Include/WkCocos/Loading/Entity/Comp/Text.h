#ifndef __DFGAME_LOADINGSCENE_ENTITY_COMP_TEXT_H__
#define __DFGAME_LOADINGSCENE_ENTITY_COMP_TEXT_H__

#include "entityx/entityx.h"

namespace dfgame
{
	namespace Loading
	{
		namespace Entity
		{
			namespace Comp
			{

				struct Text : entityx::Component<Text> {

					/**
					* @param id is a loca id
					* @param delay is the date when text must be displayed
					*/
					Text(unsigned int id, double delay) : m_id(id), m_delay(delay), m_displayed(false) {}

					unsigned int m_id;
					double m_delay;
					bool m_displayed;
				};

			}//namespace Comp
		}//namespace Entity
	}//namespace LoadingScene
}//namespace dfgame

#endif // __DFGAME_LOADINGSCENE_ENTITY_COMP_TEXT_H__
