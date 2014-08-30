#include "WkCocos/OnlineData/Systems/Storage.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

#define DB_NAME "PUBLIC"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			Storage::Storage()
			{
				m_stor_service = ::App42::App42API::BuildStorageService();
			}

			Storage::~Storage()
			{
				//StorageService::Terminate();
				//m_service = nullptr;
			}

			void Storage::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::DeleteUserData> dud;
				for (auto entity : entities->entities_with_components(dud))
				{
					if (dud->done)
					{
						entity.remove<Comp::DeleteUserData>();
						//if mask at 0 no request in this entity anymore
						//if (entity.component_mask() == 0)
						//{
						//	entity.destroy();
						//}
					}
					else if (!dud->in_progress)
					{
						m_stor_service->DeleteDocumentsByKeyValue(DB_NAME, dud->m_collection.c_str(), "user_id", dud->m_userid.c_str(), dud->m_dummy_cb);
						dud->in_progress = true;
					}

				}

				entityx::ptr<Comp::LoadUserData> lud;
				for (auto entity : entities->entities_with_components(lud))
				{
					if (lud->done)
					{
						entity.remove<Comp::LoadUserData>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!lud->in_progress)
					{
						//::App42::Query* query = ::App42::QueryBuilder::BuildQuery("user_id", lud->m_userid.c_str(), APP42_OP_EQUALS);
						//m_service->setQuery(DB_NAME, lud->m_collection, query);
						//delete query;
						CCLOG("Requesting App42 User data for : %s ", lud->m_userid.c_str());
						m_stor_service->FindDocumentByKeyValue(DB_NAME, lud->m_collection.c_str(), "user_id", lud->m_userid.c_str(), lud->m_cb);
						lud->in_progress = true;
					}

				}

				entityx::ptr<Comp::LoadEnemyData> led;
				for (auto entity : entities->entities_with_components(led))
				{
					if (led->done)
					{
						entity.remove<Comp::LoadEnemyData>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!led->in_progress)
					{
						m_stor_service->FindDocumentByKeyValue(DB_NAME, led->m_collection.c_str(), "user_id", led->m_userid.c_str(), led->m_cb);
						led->in_progress = true;
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

