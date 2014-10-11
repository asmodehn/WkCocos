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
				entityx::ptr<Comp::UpdateUserData> uud;
				for (auto entity : entities->entities_with_components(uud))
				{
					if (uud->done)
					{
						entity.remove<Comp::UpdateUserData>();
					}
					else if (!uud->in_progress)
					{
						m_stor_service->UpdateDocumentByDocId(DB_NAME, uud->m_collection.c_str(), uud->m_docid.c_str(), uud->m_user_data.c_str(), uud->m_cb);
						uud->in_progress = true;
					}

				}

				entityx::ptr<Comp::InsertUserData> iud;
				for (auto entity : entities->entities_with_components(iud))
				{
					if (iud->done)
					{
						entity.remove<Comp::InsertUserData>();
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!iud->in_progress)
					{
						::App42::App42API::setLoggedInUser(iud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, iud->m_collection.c_str(), iud->m_user_data.c_str(), iud->m_cb);
						iud->in_progress = true;
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

						::App42::Query * query = ::App42::QueryBuilder::BuildQuery(gukv->m_key.c_str(), gukv->m_value, APP42_OP_EQUALS);
						::App42::Query * querySearch = ::App42::QueryBuilder::BuildQuery("search", true, APP42_OP_EQUALS);
						::App42::Query * queryFinal = ::App42::QueryBuilder::CompoundOperator(query, APP42_OP_AND, querySearch);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, gukv->m_collection.c_str(), queryFinal, gukv->m_quantity, gukv->m_offset, gukv->m_cb);
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

						::App42::Query * queryFrom = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_from, APP42_OP_GREATER_THAN_EQUALTO);
						::App42::Query * queryTo = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_to, APP42_OP_LESS_THAN_EQUALTO);
						::App42::Query * querySearch = ::App42::QueryBuilder::BuildQuery("search", true, APP42_OP_EQUALS);
						::App42::Query * queryCompound = ::App42::QueryBuilder::CompoundOperator(queryFrom, APP42_OP_AND, queryTo);
						::App42::Query * queryFinal = ::App42::QueryBuilder::CompoundOperator(queryCompound, APP42_OP_AND, querySearch);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, guft->m_collection.c_str(), queryFinal, guft->m_quantity, guft->m_offset, guft->m_cb);
						guft->in_progress = true;
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

