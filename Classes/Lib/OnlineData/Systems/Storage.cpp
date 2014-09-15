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
					}
					else if (!dud->in_progress)
					{
						m_stor_service->DeleteDocumentsById(DB_NAME, dud->m_collection.c_str(), dud->m_docid.c_str(), dud->m_dummy_cb);
						dud->in_progress = true;
					}

				}

				entityx::ptr<Comp::FindUserData> fud;
				for (auto entity : entities->entities_with_components(fud))
				{
					if (fud->done)
					{
						entity.remove<Comp::FindUserData>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!fud->in_progress)
					{
						CCLOG("Requesting App42 search for documents : %s ", fud->m_userid.c_str());
						m_stor_service->FindAllDocuments(DB_NAME, fud->m_collection.c_str(), fud->m_cb);
						fud->in_progress = true;
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

				entityx::ptr<Comp::GetUsersWithDocs> guwd;
				for (auto entity : entities->entities_with_components(guwd))
				{
					if (guwd->done)
					{
						entity.remove<Comp::GetUsersWithDocs>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!guwd->in_progress)
					{
						CCLOG("Requesting full list of documents and retrieving their owners");
						m_stor_service->FindAllDocuments(DB_NAME, guwd->m_collection.c_str(), guwd->m_cb);
						guwd->in_progress = true;
					}

				}

				entityx::ptr<Comp::GetUsersKeyValue> gukv;
				for (auto entity : entities->entities_with_components(gukv))
				{
					if (gukv->done)
					{
						entity.remove<Comp::GetUsersKeyValue>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!gukv->in_progress)
					{
						CCLOG("Requesting list of documents with needed value and retrieving their owners");
						m_stor_service->FindDocumentByKeyValue(DB_NAME, gukv->m_collection.c_str(), gukv->m_key.c_str(), gukv->m_value.c_str(), gukv->m_cb);
						gukv->in_progress = true;
					}

				}

				entityx::ptr<Comp::GetUsersFromTo> guft;
				for (auto entity : entities->entities_with_components(guft))
				{
					if (guft->done)
					{
						entity.remove<Comp::GetUsersFromTo>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!guft->in_progress)
					{
						CCLOG("Requesting list of documents by query and retrieving their owners");

						::App42::Query * queryFrom = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_from.c_str(), APP42_OP_GREATER_THAN_EQUALTO);
						::App42::Query * queryTo = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_to.c_str(), APP42_OP_LESS_THAN_EQUALTO);
						::App42::Query * queryCompound = ::App42::QueryBuilder::CompoundOperator(queryFrom, APP42_OP_AND, queryTo);

						m_stor_service->FindDocumentsByQuery(DB_NAME, guft->m_collection.c_str(), queryCompound, guft->m_cb);
						guft->in_progress = true;
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

