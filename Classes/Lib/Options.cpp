#include "WkCocos/Options.h"
#include "cocos2d.h"
namespace WkCocos
{
		//constructors
		Options::Options(std::shared_ptr<LocalData::LocalDataManager> localdata)
			: m_localdata(localdata)
		{
		}
		
	}
