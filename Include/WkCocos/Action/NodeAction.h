#ifndef __WKCOCOS_ACTION_NODEACTION_H__
#define __WKCOCOS_ACTION_NODEACTION_H__

#include "cocos2d.h"

namespace WkCocos
{
	class Shake : public cocos2d::ActionInterval
	{
	public:
		/**
		* Destructor
		*/
		virtual ~Shake();

		/** 
		* creates the action 
		*/
		static Shake* create(float duration, int amplitude, float strength);

		//
		// Overrides
		//
		virtual Shake* clone() const override;
		virtual Shake* reverse(void) const  override;
		virtual void startWithTarget(cocos2d::Node *target) override;
		virtual void update(float completion) override;
		void initWithDuration(float duration, int amplitude, float strength);
	private:
		/**
		* Default constructor
		*/
		Shake();

		/**
		* Position we start, make sure it's the position we return
		*/
		cocos2d::Vec2		_startPos;

		/**
		* Amplitude of shaking
		*/
		int					_amplitude;

		/**
		* Speed of shaking
		*/
		float				_speed;

		/**
		*
		*/
		std::vector<cocos2d::Vec2>	_curve;

		/**
		* Percent step between interpolation
		*/
		std::vector<float>	_step;

	};

}//namespace WkCocos

#endif //__WKCOCOS_ACTION_NODEACTION_H__