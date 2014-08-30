#ifndef __WKCOCOS_ACTION_UIACTION_H__
#define __WKCOCOS_ACTION_UIACTION_H__

#include "cocos2d.h"

namespace WkCocos
{
	/**
	* Action on cocos UI object
	*/
	class UIAction : public cocos2d::ActionInterval
	{
	public:
		/**
		* Destructor
		*/
		virtual ~UIAction();

		/**
		* Override this method in child class to test the type of node.
		*/
		virtual void startWithTarget(cocos2d::Node *target) = 0;

	protected:
		/**
		* Default constructor
		*/
		UIAction();

	};

	/**
	* Action on cocos UI object
	*/
	class ProgressTo : public UIAction
	{
	public:
		/**
		* Destructor
		*/
		virtual ~ProgressTo();

		/**
		* Create an action UI Progress to
		* @param duration Lenght of action
		* @param to progress to reach
		*/
		static ProgressTo* create(float duration, float to);

		//
		// Overrides
		//
		virtual ProgressTo* clone() const override;
		virtual ProgressTo* reverse() const override;

		/**
		* Test that the node is of type cocos2d::ui::LoadingBar.
		*/
		virtual void startWithTarget(cocos2d::Node *target) override;
		virtual void update(float t) override;

	protected:
		/**
		* Default constructor
		*/
		ProgressTo();

		/**
		* init with lenght of action
		* @param duration Lenght of action
		* @param to Percent to reach
		*/
		bool initWithDuration(float duration, float to);

	protected:
		/**
		* Current Progress
		*/
		float		m_from;

		/**
		* Progress to reach
		*/
		float		m_to;

	};

}//namespace WkCocos

#endif //__WKCOCOS_ACTION_UIACTION_H__