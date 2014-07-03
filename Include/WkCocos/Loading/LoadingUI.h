#ifndef __LOADINGUI_H__
#define __LOADINGUI_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "dfgame/UI/Interface.h"

namespace dfgame
{

	/**
	 * This UI is the main menu UI
	 */
	class LoadingUI : public Interface
	{
	public:

		static const DataResource::eResourceIds resourceId = DataResource::RESOURCE_GUI_MENU_TITLE_UI_JSON;

		/**
		* Default constructor
		*/
		LoadingUI();

		/**
		* Constructor
		*/
		virtual ~LoadingUI();


		void click(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType);

	};
	
}//namespace dfgame

#endif // __LOADINGUI_H__
