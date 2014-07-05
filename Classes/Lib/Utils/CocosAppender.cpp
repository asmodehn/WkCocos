#include "WkCocos/Utils/CocosAppender.h"

#include "cocos/ui/UILayout.h"
#include "cocos/ui/UITextBMFont.h"

namespace WkCocos
{

	CocosLogAppender::CocosLogAppender(cocos2d::Node* root, const std::string& fontName)
		: _model(nullptr)
	{
		// make model
		_model = cocos2d::ui::TextBMFont::create("", fontName);

		// make tree hierarchy
		cocos2d::ui::Layout* bg = cocos2d::ui::Layout::create();
		_log = cocos2d::ui::ListView::create();
		
		// setup bg
		bg->setTouchEnabled(false);
		bg->setSize(cocos2d::Size(1920, 1080));
		bg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
		bg->setBackGroundColor(cocos2d::Color3B(20, 20, 20));
		bg->setBackGroundColorOpacity(70);
		bg->setLocalZOrder(9999);
		bg->addChild(_log);

		// setup log
		_log->setTouchEnabled(false);
		_log->setItemModel(_model);
		_log->setSize(cocos2d::Size(1920, 900));
		_log->setSizePercent(cocos2d::Vec2(1.f, 0.9f));
		_log->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
		_log->setBackGroundColor(cocos2d::Color3B(20, 20, 20));
		_log->setBackGroundColorOpacity(70);

		root->addChild(bg);
	}
	
	CocosLogAppender::~CocosLogAppender()
	{}

	LogAppender& CocosLogAppender::operator << (const LogStream & msg)
	{	
		std::stringstream str;
		str << msg;
		
		if (_logSize >= _log->getItems().size())
		{
			_log->removeItem(0);
		}

		_log->pushBackDefaultItem();
		cocos2d::ui::TextBMFont* line = static_cast<cocos2d::ui::TextBMFont*>(_log->getItem(_log->getItems().size() - 1));
		line->setString(str.str());
		
		return *this;
	}
}//namespace WkCocos
