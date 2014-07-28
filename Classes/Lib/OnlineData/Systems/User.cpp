#include "WkCocos/OnlineData/Systems/User.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "Common/App42API.h"

#define DB_NAME "PUBLIC"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			User::User()
			{
				m_service = App42API::BuildUserService();
			
			}

			User::~User()
			{
				//UserService::Terminate();
				//m_service = nullptr;
			}

			void User::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) 
			{
				entityx::ptr<Comp::Create> c;
				for (auto entity : entities->entities_with_components(c))
				{
					if (c->done)
					{
						entity.remove<Comp::Create>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!c->in_progress)
					{
						auto data = entity.component<Comp::SaveUserData>();
						if (data && !data->in_progress)
						{
							App42Object app42Object;
							app42Object.setObject("user_id", data->m_userid.c_str());
							app42Object.setObject("data", data->m_user_data.c_str());
							App42API::setDbName(DB_NAME);
							m_service->AddUserInfo(&app42Object, data->m_collection.c_str());
							data->in_progress = true;
						}
						CCLOG("Requesting App42 User creation : %s ", c->m_userid.c_str());
						m_service->CreateUser(c->m_userid.c_str(), c->m_passwd.c_str(), c->m_email.c_str(), c->m_cb);
						c->in_progress = true;
					}
				}

				entityx::ptr<Comp::Login> l;
				for (auto entity : entities->entities_with_components(l))
				{
					if (l->done)
					{
						entity.remove<Comp::Login>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!l->in_progress)
					{
						CCLOG("Requesting App42 User login : %s ", l->m_userid.c_str());
						m_service->Authenticate(l->m_userid.c_str(), l->m_passwd.c_str(), l->m_cb);
						l->in_progress = true;
					}

				}
				
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
						App42User user;
						user.userName = sud->m_userid.c_str();
						App42Object app42Object;
						app42Object.setObject("user_id", sud->m_userid.c_str());
						app42Object.setObject("data", sud->m_user_data.c_str());
						App42API::setDbName(DB_NAME);
						m_service->AddUserInfo(&app42Object, sud->m_collection.c_str());
						m_service->createOrUpdateProfile(&user, sud->m_cb);
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
						Query* query = QueryBuilder::BuildQuery("user_id", lud->m_userid.c_str(), APP42_OP_EQUALS);
						m_service->setQuery(DB_NAME, lud->m_collection, query);
						delete query;
						CCLOG("Requesting App42 User data for : %s ", lud->m_userid.c_str());
						m_service->GetUser(lud->m_userid.c_str(), lud->m_cb);
						lud->in_progress = true;
					}

				}
				
			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

