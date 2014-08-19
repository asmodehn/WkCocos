#ifndef __WKCOCOS_OPTIONS_H__
#define __WKCOCOS_OPTIONS_H__

//#include "cocos2d.h"
#include "WkCocos/LocalData/LocalDataManager.h"

#include <string>

namespace WkCocos
{
	/**
	* Hold the player information.
	*/
	class Options
	{
	public:

		/**
		* Update loop, called by cocos after Activate has been called
		*/
		void Update(float deltatime);
		
	protected:
		Options(std::shared_ptr<LocalData::LocalDataManager> localdata);
		
		std::shared_ptr<LocalData::LocalDataManager> m_localdata;

	};
} //namespace WkCocos

#endif //__WKCOCOS_OPTIONS_H__
