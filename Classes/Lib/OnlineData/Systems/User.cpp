#include "WkCocos/OnlineData/Systems/User.h"
#include "WkCocos/OnlineData/Events/Error.h"
#include "WkCocos/OnlineData/Comp/OnlineData.h"

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
						CCLOG("Create User entity lived %f seconds", c->life_time);
						entity.remove<Comp::Create>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!c->in_progress)
					{
						CCLOG("Requesting App42 User creation : %s ", c->m_userid.c_str());
						m_user_service->CreateUser(c->m_userid.c_str(), c->m_passwd.c_str(), c->m_email.c_str(), c->m_cb);
						c->in_progress = true;
					}
					else
					{
						c->life_time += dt;
						if (c->life_time > TIMEOUT)
						{
							c->timeout = true;
							events->emit<Events::Error>("user create");
						}
					}

				}

				entityx::ptr<Comp::Login> l;
				for (auto entity : entities->entities_with_components(l))
				{
					if (l->done)
					{
						CCLOG("Login entity lived %f seconds", l->life_time);
						entity.remove<Comp::Login>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!l->in_progress)
					{
						CCLOG("Requesting App42 User login : %s ", l->m_userid.c_str());
						m_user_service->Authenticate(l->m_userid.c_str(), l->m_passwd.c_str(), l->m_cb);
						l->in_progress = true;
					}
					else
					{
						l->life_time += dt;
						if (l->life_time > TIMEOUT)
						{
							l->timeout = true;
							events->emit<Events::Error>("user login");
						}
					}

				}

				entityx::ptr<Comp::LoadUserData> lud;
				for (auto entity : entities->entities_with_components(lud))
				{
					if (lud->done)
					{
						CCLOG("Load User Data entity lived %f seconds", lud->life_time);
						entity.remove<Comp::LoadUserData>();
						if (entity.component_mask() == 0)
							entity.destroy();
					}
					else if (!lud->in_progress)
					{
						CCLOG("Requesting App42 stored data in collection %s for user : %s ", lud->m_collection.c_str(), lud->m_userid.c_str());
						::App42::App42API::setLoggedInUser(lud->m_userid.c_str());
						::App42::App42API::setDbName(DB_NAME);
						m_user_service->setQuery(lud->m_collection.c_str(), NULL); //This will tell
						//App42 that you are requesting all the data linked to the above userName
						m_user_service->GetUser(lud->m_userid.c_str(), lud->m_cb);
						lud->in_progress = true;
					}
					else
					{
						lud->life_time += dt;
						if (lud->life_time > TIMEOUT)
						{
							lud->timeout = true;
							events->emit<Events::Error>("user data load");
						}
					}

				}

			}

		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos