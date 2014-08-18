#include "WkCocos/OnlineData/Systems/Storage.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

//#define DB_NAME "PUBLIC"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			Storage::Storage()
			{
				m_service = ::App42::App42API::BuildStorageService();
			}

			Storage::~Storage()
			{
				//StorageService::Terminate();
				//m_service = nullptr;
			}

			void Storage::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::SaveUserData> sud;
				for (auto entity : entities->entities_with_components(sud))
				{
					if (sud->done)
					{
						entity.remove<Comp::SaveUserData>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!sud->in_progress)
					{
						//::App42::App42User user;
						//user.userName = sud->m_userid.c_str();
						::App42::App42Object app42Object;
						app42Object.setObject("user_id", sud->m_userid.c_str());
						app42Object.setObject("data", sud->m_user_data.c_str());
						//::App42::App42API::setDbName(DB_NAME);
						//m_service->AddUserInfo(&app42Object, sud->m_collection.c_str());
						m_service->DeleteAllDocuments(DB_NAME, sud->m_collection.c_str(), sud->m_dummy_cb);
						m_service->InsertJsonDocument(DB_NAME, sud->m_collection.c_str(), &app42Object, sud->m_cb);
						sud->in_progress = true;
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
						m_service->FindAllDocuments(DB_NAME, lud->m_collection.c_str(), lud->m_cb);
						lud->in_progress = true;
					}

				}

			}
					
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

