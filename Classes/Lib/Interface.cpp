#include "WkCocos/Interface.h"
#include "WkCocos/Utils/ToolBox.h"

namespace WkCocos
{
	std::unordered_map<std::string, cocos2d::ui::Widget *> Interface::widget_cache;

	cocos2d::ui::Widget * Interface::load(std::string filepath)
	{
		auto uicached = widget_cache.find(filepath);
		if (uicached == widget_cache.end()) //ui not found in cache
		{
			cocostudio::GUIReader* reader = cocostudio::GUIReader::getInstance();

			cocos2d::ui::Widget* widget = reader->widgetFromJsonFile(filepath.c_str());
			if (widget)
			{
				widget->retain(); //we need to retain it in memory ( or cocos will drop it )
				widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(filepath, widget));
				return widget;
			}
		}
		else
		{
			//return already loaded UI
			return uicached->second;
		}
	}
	
	void Interface::forget(std::string filepath)
	{
		auto uicached = widget_cache.find(filepath);
		if (uicached != widget_cache.end()) //ui found in cache
		{
			uicached->second->release();
			widget_cache.erase(filepath);
		}
	}

	Interface::Interface():
		m_widget(nullptr),
		m_filepath("")
	{
	}

	Interface::Interface(std::string filepath)
		try : m_filepath(filepath)
		, m_widget(nullptr)
	{
		CCLOG("Interface construction");

		//if in cache it will already find it. otherwise it will populate cache
		m_widget = load(m_filepath);

		if (!m_widget)
		{
			CCLOGERROR("Interface construction ERROR !");
			throw std::logic_error("Error while constructing widget. aborting");
		}
	}
	catch (std::exception)
	{
		CCLOGERROR("ERROR BUILDING INTERFACE");
	}

	Interface::~Interface()
	{
		forget(m_filepath);
	}

} //namespace dfgame