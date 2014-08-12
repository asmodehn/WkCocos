#include "WkCocos/Action/NodeAction.h"

namespace WkCocos
{

	Shake::Shake()
		: _amplitude(0)
		, _speed(0.f)
	{}
	
	Shake::~Shake()
	{}

	Shake* Shake::create(float duration, int amplitude, float strength)
	{
		Shake *ret = new Shake();
		ret->initWithDuration(duration, amplitude, strength);
		ret->autorelease();

		return ret;
	}

	void Shake::initWithDuration(float duration, int amplitude, float strength)
	{
		ActionInterval::initWithDuration(duration);
		_amplitude = amplitude;
		_speed = strength;
		_curve.push_back(cocos2d::Vec2(0.f, 0.f));
		_curve.push_back(cocos2d::Vec2(amplitude, 0.f));
		_curve.push_back(cocos2d::Vec2(-amplitude, 0.f));
		_curve.push_back(cocos2d::Vec2(0.f, 0.f));
		_step.push_back(0.f);
		_step.push_back(0.25f);
		_step.push_back(0.75f);
		_step.push_back(1.f);
	}

	Shake* Shake::clone() const
	{
		// no copy constructor
		auto a = new Shake();
		a->initWithDuration(_duration, _amplitude, _speed);
		a->autorelease();
		return a;
	}

	Shake* Shake::reverse(void) const
	{
		return Shake::create(_duration, -_amplitude, _speed);
	}

	void Shake::startWithTarget(cocos2d::Node *target)
	{
		ActionInterval::startWithTarget(target);
		_startPos = target->getPosition();
	}

	void Shake::update(float completion)
	{
		float loop = _duration * completion;
		if (loop > 0.f)
		{
			loop = loop / _speed;
		}
		float curveCompletion = loop - static_cast<int>(loop);

		int i = 1;
		while (curveCompletion > _step[i])
		{
			++i;
		}

		float segmentRatio = (curveCompletion - _step[i - 1]) / (_step[i] - _step[i - 1]);

		cocos2d::Vec2 segment = _curve[i] - _curve[i - 1];

		_target->setPosition(_startPos + segment * segmentRatio + _curve[i - 1]);
	}

}//namespace WkCocos