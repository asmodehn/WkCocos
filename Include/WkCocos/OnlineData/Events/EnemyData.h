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
				EnemyData(std::string name, int gold, int gems, bool docs) 
					: enemy_name(name)
					, enemy_gold(gold)
					, enemy_gems(gems)
					, enemy_docs(docs)
				{ }

				std::string enemy_name;
				int enemy_gold;
				int enemy_gems;
				int enemy_docs;

			};

		}//namespace Event
	}//namespace OnlineData
}//namespace WkCocos


#endif // __DFGAME_ONLINEDATA_EVENTS_ENEMYDATA_H__
