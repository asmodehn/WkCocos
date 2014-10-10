#include "WkCocos/OnlineData/Systems/User.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

#define DB_NAME "PUBLIC"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			User::User()
			{
				m_user_service = ::App42::App42API::BuildUserService();
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
						//auto data = entity.component<Comp::SaveUserData>();
						//if (data && !data->in_progress)
						//{
						//	::App42::App42Object app42Object;
						//	app42Object.setObject("user_id", data->m_userid.c_str());
						//	app42Object.setObject("data", data->m_user_data.c_str());
						//	::App42::App42API::setDbName(DB_NAME);
						//	m_user_service->AddUserInfo(&app42Object, data->m_collection.c_str());
						//	data->in_progress = true;
						//}
						CCLOG("Requesting App42 User creation : %s ", c->m_userid.c_str());
						m_user_service->CreateUser(c->m_userid.c_str(), c->m_passwd.c_str(), c->m_email.c_str(), c->m_cb);
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
						m_user_service->Authenticate(l->m_userid.c_str(), l->m_passwd.c_str(), l->m_cb);
						l->in_progress = true;
					}

				}

				entityx::ptr<Comp::LoadUserData> lud;
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
						::App42::App42API::setLoggedInUser(lud->m_userid.c_str());
						::App42::App42API::setDbName(DB_NAME);
						m_user_service->setQuery(lud->m_collection.c_str(), NULL); //This will tell
						//App42 that you are requesting all the data linked to the above userName
						m_user_service->GetUser(lud->m_userid.c_str(), lud->m_cb);
						lud->in_progress = true;
					}

				}

				//entityx::ptr<Comp::FindUserData> fud;
				//for (auto entity : entities->entities_with_components(fud))
				//{
				//	if (fud->done)
				//	{
				//		entity.remove<Comp::FindUserData>();
				//		if (entity.component_mask() == 0)
				//		{
				//			entity.destroy();
				//		}
				//	}
				//	else if (!fud->in_progress)
				//	{
				//		CCLOG("Requesting App42 storage of user : %s ", fud->m_userid.c_str());
				//		::App42::App42API::setLoggedInUser(fud->m_userid.c_str());
				//		::App42::App42API::setDbName(DB_NAME);
				//		m_user_service->setQuery(fud->m_collection.c_str(), NULL); //This will tell
				//		//App42 that you are requesting all the data linked to the above userName
				//		m_user_service->GetUser(fud->m_userid.c_str(), fud->m_cb);
				//		fud->in_progress = true;
				//	}

				//}

			}
			
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

