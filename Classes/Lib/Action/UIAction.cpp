#include "WkCocos/Action/UIAction.h"

#include "cocos/ui/UILoadingBar.h"

namespace WkCocos
{

	UIAction::UIAction()
	{}
	
	UIAction::~UIAction()
	{}

	ProgressTo::ProgressTo()
		: m_from(0.f)
		, m_to(0.f)
	{}

	ProgressTo::~ProgressTo()
	{}

	ProgressTo* ProgressTo::create(float duration, float to)
	{
		ProgressTo* progressTo = new ProgressTo();
		progressTo->initWithDuration(duration, to);
		progressTo->autorelease();

		return progressTo;
	}

	bool ProgressTo::initWithDuration(float duration, float to)
	{
		if (ActionInterval::initWithDuration(duration))
		{
			m_to = to;
			return true;
		}
		return false;
	}

	ProgressTo* ProgressTo::clone() const
	{
		// no copy constructor
		auto a = new ProgressTo();
		a->initWithDuration(_duration, m_to);
		a->autorelease();
		return a;
	}

	ProgressTo* ProgressTo::reverse() const
	{
		CCASSERT(false, "ProgressTo doesn't support the 'reverse' method");
		return nullptr;
	}

	void ProgressTo::startWithTarget(cocos2d::Node *target)
	{
		cocos2d::ui::LoadingBar* loadingBar = dynamic_cast<cocos2d::ui::LoadingBar*>(target);
		assert(loadingBar);

		cocos2d::ActionInterval::startWithTarget(target);

		m_from = loadingBar->getPercent();
	}

	void ProgressTo::update(float t)
	{
		cocos2d::ui::LoadingBar* loadingBar = static_cast<cocos2d::ui::LoadingBar*>(_target);
		
		float diffPercent = m_to - m_from;
		loadingBar->setPercent(m_from + diffPercent * t);
	}


}//namespace wkcocos