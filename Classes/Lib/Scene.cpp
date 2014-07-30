#include "WkCocos/Scene.h"

namespace WkCocos
{
	Scene::Scene() :
		m_worldView(nullptr),
		m_uiView(nullptr)
	{
	}

	Scene::~Scene()
	{
		std::map<std::string, Interface*>::iterator currentUI = m_ui.begin();
		for (; currentUI != m_ui.end(); ++currentUI)
		{
			delete currentUI->second;
		}
		m_ui.clear();

		cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(cocos2d::Director::EVENT_AFTER_DRAW);
	}

	bool Scene::init()
	{
		// 1. super init first
		if (!cocos2d::Scene::init())
		{
			return false;
		}
		else
		{
			m_worldView = cocos2d::CameraView::create();
			m_worldView->setContentSize(getContentSize());
			m_uiView = cocos2d::CameraView::create();
			m_uiView->setContentSize(getContentSize());

			addChild(m_worldView);
			addChild(m_uiView);
			return true;
		}
	}

	void Scene::addInterface(Interface* ui)
	{
		m_uiView->addChild(ui->getRoot());

		m_ui[ui->getFilepath()] = ui;
	}

	void Scene::update(float delta)
	{
		std::map<std::string, Interface*>::iterator currentUI = m_ui.begin();
		for (; currentUI != m_ui.end(); ++currentUI)
		{
			currentUI->second->update(delta);
		}
	}

}//namespace WkCocos
