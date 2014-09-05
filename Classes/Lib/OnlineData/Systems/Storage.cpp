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
				//m_stor_service = nullptr;
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

				/*entityx::ptr<Comp::LoadUserData> lud;
				for (auto entity : entities->entities_with_components(lud))
				{
					if (lud->done)
					{
						entity.remove<Comp::LoadUserData>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!lud->in_progress)
					{
						CCLOG("Requesting App42 storage of user : %s ", lud->m_userid.c_str());
						m_stor_service->FindDocumentByKeyValue(DB_NAME, lud->m_collection.c_str(), "_$owner", lud->m_userid.c_str(), lud->m_cb);
						lud->in_progress = true;
					}

				}*/

				entityx::ptr<Comp::LoadEnemyData> led;
				for (auto entity : entities->entities_with_components(led))
				{
					if (led->done)
					{
						entity.remove<Comp::LoadEnemyData>();
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

				entityx::ptr<Comp::SaveUserData> sud;
				for (auto entity : entities->entities_with_components(sud))
				{
					if (sud->done)
					{
						entity.remove<Comp::SaveUserData>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!sud->in_progress)
					{
						::App42::App42API::setLoggedInUser(sud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, sud->m_collection.c_str(), sud->m_user_data.c_str(), sud->m_cb);
						sud->in_progress = true;
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

