#include "WkCocos/OnlineData/Systems/Storage.h"
#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

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
						CCLOG("Update User Data entity lived %f seconds", uud->life_time);
						entity.remove<Comp::UpdateUserData>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!uud->in_progress)
					{
						m_stor_service->UpdateDocumentByDocId(DB_NAME, uud->m_collection.c_str(), uud->m_docid.c_str(), uud->m_user_data.c_str(), uud->m_cb);
						uud->in_progress = true;
					}
					else
					{
						uud->life_time += dt;
						if (uud->life_time > TIMEOUT && !uud->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							uud->timeout = true;
							events->emit<Events::Error>(entity.id(), "update user data");
						}
					}

				}

				entityx::ptr<Comp::InsertUserData> iud;
				for (auto entity : entities->entities_with_components(iud))
				{
					if (iud->done)
					{
						CCLOG("Insert User Data entity lived %f seconds", iud->life_time);
						entity.remove<Comp::InsertUserData>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!iud->in_progress)
					{
						::App42::App42API::setLoggedInUser(iud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, iud->m_collection.c_str(), iud->m_user_data.c_str(), iud->m_cb);
						iud->in_progress = true;
					}
					else
					{
						iud->life_time += dt;
						if (iud->life_time > TIMEOUT && !iud->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							iud->timeout = true;
							events->emit<Events::Error>(entity.id(), "insert user data");
						}
					}

				}

				entityx::ptr<Comp::GetUsersKeyValue> gukv;
				for (auto entity : entities->entities_with_components(gukv))
				{
					if (gukv->done)
					{
						CCLOG("Get Data By Key Value entity lived %f seconds", gukv->life_time);
						entity.remove<Comp::GetUsersKeyValue>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!gukv->in_progress)
					{
						CCLOG("Requesting list of documents with needed value and retrieving their owners");

						::App42::Query * query = ::App42::QueryBuilder::BuildQuery(gukv->m_key.c_str(), gukv->m_value, APP42_OP_EQUALS);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, gukv->m_collection.c_str(), query, gukv->m_quantity, gukv->m_offset, gukv->m_cb);
						gukv->in_progress = true;
					}
					else
					{
						gukv->life_time += dt;
						if (gukv->life_time > TIMEOUT && !gukv->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							gukv->timeout = true;
							events->emit<Events::Error>(entity.id(), "get data by key and value");
						}
					}

				}

				entityx::ptr<Comp::GetUsersFromTo> guft;
				for (auto entity : entities->entities_with_components(guft))
				{
					if (guft->done)
					{
						CCLOG("Get Data By Query entity lived %f seconds", guft->life_time);
						entity.remove<Comp::GetUsersFromTo>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!guft->in_progress)
					{
						CCLOG("Requesting list of documents by query and retrieving their owners");

						::App42::Query * queryFrom = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_from, APP42_OP_GREATER_THAN_EQUALTO);
						::App42::Query * queryTo = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_to, APP42_OP_LESS_THAN_EQUALTO);
						::App42::Query * queryCompound = ::App42::QueryBuilder::CompoundOperator(queryFrom, APP42_OP_AND, queryTo);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, guft->m_collection.c_str(), queryCompound, guft->m_quantity, guft->m_offset, guft->m_cb);
						guft->in_progress = true;
					}
					else
					{
						guft->life_time += dt;
						if (guft->life_time > TIMEOUT && !guft->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							guft->timeout = true;
							events->emit<Events::Error>(entity.id(), "get data by query");
						}
					}

				}

				entityx::ptr<Comp::AllDocsPaging> adp;
				for (auto entity : entities->entities_with_components(adp))
				{
					if (adp->done)
					{
						CCLOG("Docs List Data entity lived %f seconds", adp->life_time);
						entity.remove<Comp::AllDocsPaging>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!adp->in_progress)
					{
						CCLOG("Requesting list of documents page by page");
						m_stor_service->FindAllDocuments(DB_NAME, adp->m_collection.c_str(), adp->m_quantity, adp->m_offset, adp->m_cb);
						adp->in_progress = true;
					}
					else
					{
						adp->life_time += dt;
						if (adp->life_time > TIMEOUT && !adp->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							adp->timeout = true;
							events->emit<Events::Error>(entity.id(), "docs list");
						}
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

