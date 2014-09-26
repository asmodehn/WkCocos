#ifndef __WKCOCOS_INTERFACE_H__
#define __WKCOCOS_INTERFACE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "WebView/UIWebView.h"

#include <unordered_map>
#include <unordered_set>

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
		* @warning : this method should NOT be used : facade methods in this class should be preferred.
		*/
		inline cocos2d::ui::Widget* getRoot()
		{
			return m_widget;
		}
		
		/**
		* set the position of the widget
		*/
		inline void setPosition(const cocos2d::Vec2& position)
		{
			m_widget->setPosition(position);
		}

		/**
		* enabling the widget
		*/
		inline void setEnabled(bool enable)
		{
			return m_widget->setEnabled(enable);
		}

		/**
		* showing the widget ( and a webview if webview is in its children )
		*/
		void setVisible(bool visible);

		/**
		* Adds a child
		*/
		virtual void addChild(cocos2d::Node * child);
		virtual void addChild(cocos2d::experimental::ui::WebView * child);

		/**
		* Removes a child
		*/
		virtual void removeChild(cocos2d::Node * child);
		virtual void removeChild(cocos2d::experimental::ui::WebView * child);

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
		std::unordered_set<cocos2d::experimental::ui::WebView *> m_webviews;

		/**
		* UI Filepath
		*/
		std::string		m_filepath;

		/**
		* Preloaded memory
		*/
		static std::unordered_map<std::string, cocos2d::ui::Widget *> widget_cache;

		friend class Scene;
	};
	
}//namespace dfgame

#endif // __WKCOCOS_INTERFACE_H__