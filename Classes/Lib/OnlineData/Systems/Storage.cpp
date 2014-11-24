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
						m_stor_service->UpdateDocumentByDocId(DB_NAME, uud->m_collection.c_str(), uud->m_docid.c_str(), uud->m_user_data.c_str(), [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
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
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
                    {
                        events->emit<Events::Error>(entity.id(), "update user data");
                        //removing entity to make sure it doesnt get called later
                        entity.destroy();
                    }

				}

				entityx::ptr<Comp::InsertUserData> iud;
				for (auto entity : entities->entities_with_components(iud, pu))
				{
					if (!pu->in_progress)
					{
						::App42::App42API::setLoggedInUser(iud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, iud->m_collection.c_str(), iud->m_user_data.c_str(), [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::InsertUserData> eciud = entity.component<Comp::InsertUserData>();
									if (eciud && eciud->m_cb) eciud->m_cb(data);
								}
								//job is done
								CCLOG("Insert User Data entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "insert user data");
						entity.destroy();
					}

				}

				entityx::ptr<Comp::GetUsersKeyValue> gukv;
				for (auto entity : entities->entities_with_components(gukv, pu))
				{
					if (!pu->in_progress)
					{
						::App42::Query * query = ::App42::QueryBuilder::BuildQuery(gukv->m_key.c_str(), gukv->m_value, APP42_OP_EQUALS);
						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, gukv->m_collection.c_str(), query, gukv->m_quantity, gukv->m_offset, [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::GetUsersKeyValue> ecgukv = entity.component<Comp::GetUsersKeyValue>();
									if (ecgukv && ecgukv->m_cb) ecgukv->m_cb(data);
								}
								//job is done
								CCLOG("Get Data By Key Value entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "get data by key and value");
						entity.destroy();
					}

				}

				entityx::ptr<Comp::GetUsersFromTo> guft;
				for (auto entity : entities->entities_with_components(guft, pu))
				{
					if (!pu->in_progress)
					{
						::App42::Query * queryFrom = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_from, APP42_OP_GREATER_THAN_EQUALTO);
						::App42::Query * queryTo = ::App42::QueryBuilder::BuildQuery(guft->m_key.c_str(), guft->m_to, APP42_OP_LESS_THAN_EQUALTO);
						::App42::Query * queryCompound = ::App42::QueryBuilder::CompoundOperator(queryFrom, APP42_OP_AND, queryTo);
						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, guft->m_collection.c_str(), queryCompound, guft->m_quantity, guft->m_offset, [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::GetUsersFromTo> ecguft = entity.component<Comp::GetUsersFromTo>();
									if (ecguft && ecguft->m_cb) ecguft->m_cb(data);
								}
								//job is done
								CCLOG("Get Data By Query entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "get data by query");
						entity.destroy();
					}

				}

				entityx::ptr<Comp::AllDocsPaging> adp;
				for (auto entity : entities->entities_with_components(adp, pu))
				{
					if (!pu->in_progress)
					{
						m_stor_service->FindAllDocuments(DB_NAME, adp->m_collection.c_str(), adp->m_quantity, adp->m_offset, [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::AllDocsPaging> ecadp = entity.component<Comp::AllDocsPaging>();
									if (ecadp && ecadp->m_cb) ecadp->m_cb(data);
								}
								//job is done
								CCLOG("Docs List Data entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "docs list");
						entity.destroy();
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

