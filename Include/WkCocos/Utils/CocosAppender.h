#ifndef __WKCOCOS_UTILS_COCOSAPPENDER_H__
#define __WKCOCOS_UTILS_COCOSAPPENDER_H__

#include "WkCocos/Utils/log/logappender.h"

//#include "cocos/2d/CCNode.h"
#include "cocos/ui/UIWidget.h"
#include "cocos/ui/UIListView.h"
#include "cocos/ui/UICheckBox.h"

namespace WkCocos
{
	/**
	* Cocos appender.
	* Display the log on a ui on screen.
	*/
	class CocosLogAppender : public LogAppender
	{
	public:
		struct CheckBoxRes
		{
			enum Type
			{
				BG,
				BG_SEL,
				BG_DIS,
				MARK,
				CROSS,
				LAST
			};

			std::string								m_textureName[LAST];
			cocos2d::ui::Widget::TextureResType		m_textureType[LAST];
			CheckBoxRes()
			{
				for (int i = 0; i < LAST; ++i)
				{
					m_textureName[i] = "";
					m_textureType[i] = cocos2d::ui::Widget::TextureResType::LOCAL;
				}
			}
		};

	public:
		/**
		* Default construtor
		*/
		CocosLogAppender() = delete;

		/**
		* construtor
		* @param root Node to attach to
		* @param fontName Name of the font to use (must be .fnt type)
		* @param res Resource for checkbox used for toggling input on log
		*/
		CocosLogAppender(cocos2d::Node* root, const std::string& fontName, const CheckBoxRes& res, cocos2d::Size widgetSize);

		/**
		* Destructor
		*/
		virtual ~CocosLogAppender();

		/**
		* Write the log
		*/
		LogAppender& operator << (const LogStreamBuf & msg);

		/**
		* Set the log maximum line
		* @param lineCount Number of line before flushing
		*/
		inline void setLogSize(int lineCount) { _logSize = lineCount; }

		/**
		* Toggle log scroll
		*/
		void toggleScroll(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);

	private:

		cocos2d::ui::ListView*	_log;
		cocos2d::ui::Widget*	_model;
		cocos2d::Size			_UISize;

		static const int		_MAX_LINE = 1000;
		int						_logSize;
	};

	/**
	* Cocos console appender.
	* Display the log on console using cocos macros.
	*/
	class CCocosLogAppender : public LogAppender
	{

	public:
		/**
		* Default construtor
		*/
		CCocosLogAppender();

		/**
		* Destructor
		*/
		virtual ~CCocosLogAppender();

		/**
		* Write the log
		*/
		LogAppender& operator << (const LogStreamBuf & msg);
	};

}//namespace WkCocos

#endif //__WKCOCOS_UTILS_COCOSAPPENDER_H__