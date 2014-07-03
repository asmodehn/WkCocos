#ifndef __DISPLAY_MANAGER_H__
#define __DISPLAY_MANAGER_H__

#include "cocos2d.h"
#include "dfgame/UI/Interface.h"

#include "generated/dataBase/DataLoading.h"

namespace dfgame
{
	/*
	class DisplayManager : public Manager {
	public:
		explicit Level(filename string) : filename_(filename) {}

	protected:
		void configure() {
			system_manager->add<DebugSystem>();
			system_manager->add<MovementSystem>();
			system_manager->add<CollisionSystem>();
		};

		void initialize() {
			level_.load(filename_);

			for (auto e : level.entity_data()) {
				entityx::Entity entity = entity_manager->create();
				entity.assign<Position>(rand() % 100, rand() % 100);
				entity.assign<Direction>((rand() % 10) - 5, (rand() % 10) - 5);
			}
		}

		void update(double dt) {
			system_manager->update<MovementSystem>(dt);
			system_manager->update<CollisionSystem>(dt);
		}

		string filename_;
		Level level_;
	};
	*/

} // namespace dfgame 

#endif //__DISPLAY_MANAGER_H__
