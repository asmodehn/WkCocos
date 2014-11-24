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
				entityx::ptr<Comp::ProgressUpdate> pu;

				entityx::ptr<Comp::UpdateUserData> uud;
				for (auto entity : entities->entities_with_components(uud, pu))
				{
                    if (!pu->in_progress)
					{
						m_stor_service->UpdateDocumentByDocId(  DB_NAME
                                                                , uud->m_collection.c_str()
                                                                , uud->m_docid.c_str()
                                                                , uud->m_user_data.c_str()
                                                                , [entity](void* data) mutable {//we need only entity here which is just an ID ( and can mutate)
                                                                        if (entity.valid()) //to be on the safe side
                                                                        {
                                                                            entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
                                                                            if (ecpu && !ecpu->life_time < TIMEOUT)
                                                                            {
                                                                                entityx::ptr<Comp::UpdateUserData> ecuud = entity.component<Comp::UpdateUserData>();
                                                                                if (ecuud && ecuud->m_cb) ecuud->m_cb(data);
                                                                            }
                                                                            //job is done
                                                                            CCLOG("Update User Data entity lived %f seconds", ecpu->life_time);
                                                                            entity.destroy();
                                                                        }
                                                                    }
                                                                );
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT) // make sure error is emitted only once before i find out how to stop entity
                    {
                        events->emit<Events::Error>(entity.id(), "update user data");
                        //removing entity to make sure it doesnt get called later
                        entity.destroy();
                    }

				}

				entityx::ptr<Comp::InsertUserData> iud;
				for (auto entity : entities->entities_with_components(iud, pu))
				{
					if (iud->done)
					{
						CCLOG("Insert User Data entity lived %f seconds", pu->life_time);
						entity.remove<Comp::InsertUserData>();
						entity.remove<Comp::ProgressUpdate>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!pu->in_progress)
					{
						::App42::App42API::setLoggedInUser(iud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, iud->m_collection.c_str(), iud->m_user_data.c_str(), iud->m_cb);
						pu->in_progress = true;
					}
					else
					{
						if (pu->life_time > TIMEOUT && !iud->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							iud->timeout = true;
							events->emit<Events::Error>(entity.id(), "insert user data");
						}
					}

				}

				entityx::ptr<Comp::GetUsersKeyValue> gukv;
				for (auto entity : entities->entities_with_components(gukv, pu))
				{
					if (gukv->done)
					{
						CCLOG("Get Data By Key Value entity lived %f seconds", pu->life_time);
						entity.remove<Comp::GetUsersKeyValue>();
						entity.remove<Comp::ProgressUpdate>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!pu->in_progress)
					{
						CCLOG("Requesting list of documents with needed value and retrieving their owners");

						::App42::Query * query = ::App42::QueryBuilder::BuildQuery(gukv->m_key.c_str(), gukv->m_value, APP42_OP_EQUALS);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, gukv->m_collection.c_str(), query, gukv->m_quantity, gukv->m_offset, gukv->m_cb);
						pu->in_progress = true;
					}
					else
					{
						if (pu->life_time > TIMEOUT && !gukv->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							gukv->timeout = true;
							events->emit<Events::Error>(entity.id(), "get data by key and value");
						}
					}

				}

				entityx::ptr<Comp::GetUsersFromTo> guft;
				for (auto entity : entities->entities_with_components(guft, pu))
				{
					if (guft->done)
					{
						CCLOG("Get Data By Query entity lived %f seconds", pu->life_time);
						entity.remove<Comp::GetUsersFromTo>();
						entity.remove<Comp::ProgressUpdate>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!pu->in_progress)
					{
						CCLOG("Requesting list of documents by query and retrieving their owners");

						::App42::Query * queryFrom = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_from, APP42_OP_GREATER_THAN_EQUALTO);
						::App42::Query * queryTo = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_to, APP42_OP_LESS_THAN_EQUALTO);
						::App42::Query * queryCompound = ::App42::QueryBuilder::CompoundOperator(queryFrom, APP42_OP_AND, queryTo);

						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, guft->m_collection.c_str(), queryCompound, guft->m_quantity, guft->m_offset, guft->m_cb);
						pu->in_progress = true;
					}
					else
					{
						if (pu->life_time > TIMEOUT && !guft->timeout) // make sure error is emitted only once before i find out how to stop entity
						{
							guft->timeout = true;
							events->emit<Events::Error>(entity.id(), "get data by query");
						}
					}

				}

				entityx::ptr<Comp::AllDocsPaging> adp;
				for (auto entity : entities->entities_with_components(adp, pu))
				{
					if (adp->done)
					{
						CCLOG("Docs List Data entity lived %f seconds", pu->life_time);
						entity.remove<Comp::AllDocsPaging>();
						entity.remove<Comp::ProgressUpdate>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!pu->in_progress)
					{
						CCLOG("Requesting list of documents page by page");
						m_stor_service->FindAllDocuments(DB_NAME, adp->m_collection.c_str(), adp->m_quantity, adp->m_offset, adp->m_cb);
						pu->in_progress = true;
					}
					else
					{
						if (pu->life_time > TIMEOUT && !adp->timeout) // make sure error is emitted only once before i find out how to stop entity
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

