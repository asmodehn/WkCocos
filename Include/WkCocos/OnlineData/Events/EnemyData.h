#ifndef __DFGAME_ONLINEDATA_EVENTS_ENEMYDATA_H__
#define __DFGAME_ONLINEDATA_EVENTS_ENEMYDATA_H__

#include "entityx/entityx.h"
//#include "WkCocos/OnlineData/Comp/OnlineData.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Events
		{
			struct EnemyData : public entityx::Event<EnemyData>
			{
				EnemyData(std::string name, int gold, int gems) 
					: enemy_name(name)
					, enemy_gold(gold)
					, enemy_gems(gems)
				{ }

				std::string enemy_name;
				int enemy_gold;
				int enemy_gems;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_ENEMYDATA_H__
