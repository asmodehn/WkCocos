#ifndef __WKCOCOS_UTILS_COCOSAPPENDER_H__
#define __WKCOCOS_UTILS_COCOSAPPENDER_H__

#include "WkCocos/Utils/log/logappender.h"

#include "cocos/2d/CCNode.h"
#include "cocos/ui/UIWidget.h"
#include "cocos/ui/UIListView.h"

namespace WkCocos
{
	/**
	* Cocos appender.
	* Display the log on a ui on screen.
	*/
	class CocosLogAppender : public LogAppender
	{
	public:
		/**
		* Default construtor
		*/
		CocosLogAppender() = delete;

		/**
		* construtor
		*/
		CocosLogAppender(cocos2d::Node* root, const std::string& fontName);

		/**
		* Destructor
		*/
		virtual ~CocosLogAppender();

		/**
		* Write the log
		*/
		LogAppender& operator << (const LogStream & msg);

		/**
		* Set the log maximum line
		* @param lineCount Number of line before flushing
		*/
		inline void setLogSize(int lineCount) { _logSize = lineCount; }
	private:

		cocos2d::ui::ListView*	_log;
		cocos2d::ui::Widget*	_model;

		static const int		_MAX_LINE = 1000;
		int						_logSize;
	};

}//namespace WkCocos

#endif //__WKCOCOS_UTILS_COCOSAPPENDER_H__