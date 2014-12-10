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
		static cocos2d::ui::Widget * load(std::string filepath, bool reuse_from_cache = true);
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
		virtual void setVisible(bool visible);

		/**
		* check if the widget is visible
		*/
		virtual bool isVisible() const;

		/**
		* Adds a child
		*/
		virtual void addChild(cocos2d::Node * child);

        /**
         * Gets a child from the container with its name
         *
         * @param name   An identifier to find the child node.
         *
         * @return a Node object whose name equals to the input parameter
         *
         * @since v3.2
         */
        virtual cocos2d::Node* getChildByName(const std::string& name) const;

        /**
         * Gets a child from the container with its name that can be cast to Type T
         *
         * @param name   An identifier to find the child node.
         *
         * @return a Node with the given name that can be cast to Type T
        */
        template <typename T>
        inline T getChildByName(const std::string& name) const { return static_cast<T>(getChildByName(name)); }

		/**
		* Removes a child
		*/
		virtual void removeChild(cocos2d::Node * child);

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

		/**
		* Set of webViews that we need to keep in check to manage visibility
		*/
		std::unordered_set<cocos2d::experimental::ui::WebView *> m_webviews;

		/**
		* Utility method that recursively parse node tree to add webviews to the list
		*/
		void addWebView(cocos2d::Node * child);
		void removeWebView(cocos2d::Node * child);

		friend class Scene;
	};

}//namespace dfgame

#endif // __WKCOCOS_INTERFACE_H__
