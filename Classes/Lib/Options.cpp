#include "WkCocos/Options.h"

namespace WkCocos
{
		//constructors
		Options::Options(std::shared_ptr<LocalData::LocalDataManager> localdata)
			: m_localdata(localdata)
		{
			//registering option class in cocos update loop
			cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&Options::Update, this, std::placeholders::_1), this, 1.f / 15, false, "options_update");

		}

		//member methods
		void Options::Update(float deltatime)
		{
			if (m_localdata)
			{
				m_localdata->update(deltatime);
			}
		}

	}
