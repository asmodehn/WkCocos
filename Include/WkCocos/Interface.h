#ifndef __WKCOCOS_INTERFACE_H__
#define __WKCOCOS_INTERFACE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include <unordered_map>

namespace WkCocos
{
	/**
	 * This is mother class of all UI.
	 */
	class Interface
	{
	public:

		/**
		* Loads a UI resource into memory.
		* @return pointer to the widget in preloaded memory.
		*/
		static cocos2d::ui::Widget * load(std::string filepath);
		static void forget(std::string filepath);
		
		/**
		* Constructor
		* @param filepath the path of the file
		*/
		Interface(std::string filepath);

		/**
		* Destructor
		* This will removes the Interface that was preloaded into memory for this Instance.
		*/
		virtual ~Interface();

		/**
		* Get the root of interface
		* @return interface root widget
		*/
		inline cocos2d::ui::Widget* getRoot()
		{
			return m_widget;
		}

		/**
		* get widget in hierarchy
		* @param name Name of the widget to find
		* @root current level of hierarchy, if null, use m_widget
		*/
		cocos2d::ui::Widget* getWidgetInHierarchy(const std::string& name, cocos2d::ui::Widget* root = nullptr);

		/**
		* Get ui ID.
		* @return UI ID
		*/
		inline const std::string& getFilepath() const { return m_filepath; }

		/**
		* update function that is called by Scene to update UI everyframe if needed.
		*/
		virtual void update(float delta);

	protected:
		/**
		* Default constructor
		* it will generate an empty Interface
		*/
		Interface();
	
		/**
		* Widget root
		*/
		cocos2d::ui::Widget *m_widget;

		/**
		* UI Filepath
		*/
		std::string		m_filepath;

		/**
		* Preloaded memory
		*/
		static std::unordered_map<std::string, cocos2d::ui::Widget *> widget_cache;

	};
	
}//namespace dfgame

#endif // __WKCOCOS_INTERFACE_H__