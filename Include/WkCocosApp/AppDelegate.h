#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "WkCocos/WkCocos.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

private:
	/**
	* Log appender: Console
	*/
	WkCocos::CLogAppender*		m_consoleApp;

	/**
	* Log appender: File
	*/
	WkCocos::FileLogAppender*	m_fileApp;

	/**
	* Log appender: Cocos
	*/
	WkCocos::CocosLogAppender*	m_cocosApp;
};

#endif // _APP_DELEGATE_H_

