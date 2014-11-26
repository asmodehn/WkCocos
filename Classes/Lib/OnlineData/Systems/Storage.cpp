#include "WkCocos/OnlineData/Systems/Storage.h"
#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Events/PlayersList.h"
#include "WkCocos/OnlineData/Events/DocsList.h"
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
						m_stor_service->UpdateDocumentByDocId(DB_NAME, uud->m_collection.c_str(), uud->m_docid.c_str(), uud->m_user_data.c_str(), [entity, events](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::UpdateUserData> ecuud = entity.component<Comp::UpdateUserData>();
									::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
									if (userdata->isSuccess)
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([ecuud, userdata](){
											ecuud->m_cb(userdata->storages.back().collectionName, userdata->storages.back().jsonDocArray.back().getDocId(), userdata->getBody());
										});
									}
									else
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity, userdata](){
											events->emit<Events::Error>(entity.id(), userdata);
										});
									}
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
						cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity](){
							events->emit<Events::Error>(entity.id(), "update user data");
						});
                        entity.destroy();
                    }

				}

				entityx::ptr<Comp::InsertUserData> iud;
				for (auto entity : entities->entities_with_components(iud, pu))
				{
					if (!pu->in_progress)
					{
						::App42::App42API::setLoggedInUser(iud->m_userid.c_str());
						m_stor_service->InsertJsonDocument(DB_NAME, iud->m_collection.c_str(), iud->m_user_data.c_str(), [entity, events](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::InsertUserData> eciud = entity.component<Comp::InsertUserData>();
									::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
									if (userdata->isSuccess)
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([eciud, userdata](){
											eciud->m_cb(userdata->storages.back().collectionName, userdata->storages.back().jsonDocArray.back().getDocId(), userdata->getBody());
										});
									}
									else
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity, userdata](){
											events->emit<Events::Error>(entity.id(), userdata);
										});
									}
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
						cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity](){
							events->emit<Events::Error>(entity.id(), "insert user data");
						});
						entity.destroy();
					}

				}

				entityx::ptr<Comp::GetUsersKeyValue> gukv;
				for (auto entity : entities->entities_with_components(gukv, pu))
				{
					if (!pu->in_progress)
					{
						::App42::Query * query = ::App42::QueryBuilder::BuildQuery(gukv->m_key.c_str(), gukv->m_value, APP42_OP_EQUALS);
						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, gukv->m_collection.c_str(), query, gukv->m_quantity, gukv->m_offset, [entity, events](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::GetUsersKeyValue> ecgukv = entity.component<Comp::GetUsersKeyValue>();
									::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
									std::map<std::string, std::string> doc;
									if (userdata->isSuccess)
									{
										auto firstStorage = userdata->storages.begin();
										auto jsonDocArray = firstStorage->jsonDocArray;
										for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
										{
											doc[it->getOwner()] = it->getJsonDoc();
										}
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, doc, firstStorage](){
											events->emit<Events::PlayersList>(doc, (int)firstStorage->recordCount);
										});
									}
									else
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, doc](){
											events->emit<Events::PlayersList>(doc, 0);
										});
									}
								}
								CCLOG("Get Data By Key Value entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity](){
							events->emit<Events::Error>(entity.id(), "get data by key and value");
						});
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
						m_stor_service->FindDocumentsByQueryWithPaging(DB_NAME, guft->m_collection.c_str(), queryCompound, guft->m_quantity, guft->m_offset, [entity, events](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::GetUsersFromTo> ecguft = entity.component<Comp::GetUsersFromTo>();
									::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
									std::map<std::string, std::string> doc;
									if (userdata->isSuccess)
									{
										auto firstStorage = userdata->storages.begin();
										auto jsonDocArray = firstStorage->jsonDocArray;
										for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
										{
											doc[it->getOwner()] = it->getJsonDoc();
										}
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, doc, firstStorage](){
											events->emit<Events::PlayersList>(doc, (int)firstStorage->recordCount);
										});
									}
									else
									{
										cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, doc](){
											events->emit<Events::PlayersList>(doc, 0);
										});
									}
								}
								CCLOG("Get Data By Query entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity](){
							events->emit<Events::Error>(entity.id(), "get data by query");
						});
						entity.destroy();
					}

				}

				entityx::ptr<Comp::AllDocsPaging> adp;
				for (auto entity : entities->entities_with_components(adp, pu))
				{
					if (!pu->in_progress)
					{
						m_stor_service->FindAllDocuments(DB_NAME, adp->m_collection.c_str(), adp->m_quantity, adp->m_offset, [entity, events](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::AllDocsPaging> ecadp = entity.component<Comp::AllDocsPaging>();
									::App42::App42StorageResponse* userdata = static_cast<::App42::App42StorageResponse*>(data);
									std::vector<std::map<std::string, std::string>> table;
									if (userdata->isSuccess)
									{
										std::map<std::string, std::string> line;
										auto jsonDocArray = userdata->storages.begin()->jsonDocArray;
										for (std::vector<::App42::JSONDocument>::iterator it = jsonDocArray.begin(); it != jsonDocArray.end(); ++it)
										{
											line["Document_ID"] = it->getDocId();
											line["JSON_Document"] = it->getJsonDoc();
											line["Owner"] = it->getOwner();
											line["Created_On"] = it->getCreatedAt();
											line["Updated_On"] = it->getUpdatedAt();
											table.push_back(line);
										}
									}
									cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, table](){
										events->emit<Events::DocsList>(table);
									});
								}
								CCLOG("Docs List Data entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([events, entity](){
							events->emit<Events::Error>(entity.id(), "docs list");
						});
						entity.destroy();
					}

				}

			}
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

