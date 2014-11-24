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
				entityx::ptr<Comp::ProgressUpdate> pu;

				entityx::ptr<Comp::Create> c;
				for (auto entity : entities->entities_with_components(c, pu))
				{
					if (!pu->in_progress)
					{
						m_user_service->CreateUser(c->m_userid.c_str(), c->m_passwd.c_str(), c->m_email.c_str(), [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::Create> ecc = entity.component<Comp::Create>();
									if (ecc && ecc->m_cb) ecc->m_cb(data);
								}
								//job is done
								CCLOG("Create User entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "user create");
						entity.destroy();
					}

				}

				entityx::ptr<Comp::Login> l;
				for (auto entity : entities->entities_with_components(l, pu))
				{
					if (!pu->in_progress)
					{
						m_user_service->Authenticate(l->m_userid.c_str(), l->m_passwd.c_str(), [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::Login> ecl = entity.component<Comp::Login>();
									if (ecl && ecl->m_cb) ecl->m_cb(data);
								}
								//job is done
								CCLOG("Login User entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "user login");
						entity.destroy();
					}

				}

				entityx::ptr<Comp::LoadUserData> lud;
				for (auto entity : entities->entities_with_components(lud, pu))
				{
					if (!pu->in_progress)
					{
						::App42::App42API::setLoggedInUser(lud->m_userid.c_str());
						::App42::App42API::setDbName(DB_NAME);
						m_user_service->setQuery(lud->m_collection.c_str(), NULL); //This will tell
						//App42 that you are requesting all the data linked to the above userName
						m_user_service->GetUser(lud->m_userid.c_str(), [entity](void* data) mutable
						{//we need only entity here which is just an ID (and can mutate)
							if (entity.valid()) //to be on the safe side
							{
								entityx::ptr<Comp::ProgressUpdate> ecpu = entity.component<Comp::ProgressUpdate>();
								if (ecpu && !ecpu->life_time < TIMEOUT)
								{
									entityx::ptr<Comp::LoadUserData> eclud = entity.component<Comp::LoadUserData>();
									if (eclud && eclud->m_cb) eclud->m_cb(data);
								}
								//job is done
								CCLOG("Load User Data entity lived %f seconds", ecpu->life_time);
								entity.destroy();
							}
						});
						pu->in_progress = true;
					}
					else if (pu->life_time > TIMEOUT)
					{
						events->emit<Events::Error>(entity.id(), "user data load");
						entity.destroy();
					}

				}

			}

		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos