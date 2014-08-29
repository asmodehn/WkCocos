#ifndef __WKCOCOS_SCENE_H__
#define __WKCOCOS_SCENE_H__

//#include "cocos2d.h"
#include "WkCocos/Interface.h"

namespace WkCocos
{
	/**
	* Generic behavior not handle by cocos for our custom scenes.
	*/
	class Scene : public cocos2d::Scene
	{
	public:
		/**
		* implement the "static create()" method manually
		*/
		CREATE_FUNC(Scene);

		/**
		* Destructor
		*/
		virtual ~Scene();

		/**
		* Initialize the scene
		*/
		virtual bool init();

		/**
		* Add UI to manage
		* @param ui Interface to manage
		*/
		void addInterface(Interface* ui);

		/**
		* Get UI loaded
		* @param filepath filepath or string id of the UI to fetch
		*/

		template <typename UI = Interface>
		inline UI* getInterface(const std::string& filepath) { return dynamic_cast<UI*>(m_ui.at(filepath)); }

		/**
		* Get the world view
		* @return the world view
		*/
		//inline cocos2d::CameraView* getWorldView() const { return m_worldView; }

		/**
		* Get the ui view
		* @return the ui view
		*/
		//inline cocos2d::CameraView* getUIView() const { return m_uiView; }

		/**
		* Override update to trigger update in all UIs
		*/
		virtual void update(float delta) override;

	protected:
		/**
		* Default constructor.
		* To create a new scene, call "create"
		*/
		Scene();

		/**
		* UI loaded map
		*/
		std::map<std::string, Interface*>		m_ui;

		/**
		* View use for object of the world, such as character
		*/
		//cocos2d::CameraView*		m_worldView;

		/**
		* View use for UI, not supposed to move with the world camera
		*/
		//cocos2d::CameraView*		m_uiView;
	};
}//namespace WkCocos

#endif //__WKCOCOS_SCENE_H__