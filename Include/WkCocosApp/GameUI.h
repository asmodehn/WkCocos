#ifndef __GAMEUI_H__
#define __GAMEUI_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"

/**
* This UI is the main menu UI
*/
class GameUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	GameUI();

	/**
	* Destructor
	*/
	virtual ~GameUI();
	
	void update(float delta);

protected:

	bool circlesToVanish[7][9];
	bool circlesToFall[7][10];
	cocos2d::DrawNode* circles[7][10];
	bool circlesPresent[7][10];
	bool circlesVanishing, circlesFalling, direction;
	bool circlesTurn = false;
	float circlesCoords[7][10][2];
	int circlesColors[7][10];
	cocos2d::Color4F colors[7];
	cocos2d::DrawNode* cursor;
	float cursorCoords[12][8][2];
	int cpx, cpy;
	int pm[12][8][3][2];
	float circleSize;
	int coef = 4;

	void circleCreate(int i, int j);
	void vanishCheck();
	void commonTouch();
	
	cocos2d::ui::Button* m_rotateRightButton;
	cocos2d::ui::Button* m_rotateLeftButton;
	cocos2d::ui::Button* m_moveRightButton;
	cocos2d::ui::Button* m_moveLeftButton;
	cocos2d::ui::Button* m_moveUpButton;
	cocos2d::ui::Button* m_moveDownButton;

	void rotateRightCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void rotateLeftCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void moveRightCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void moveLeftCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void moveUpCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);
	void moveDownCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input);

};

#endif // __GAMEUI_H__