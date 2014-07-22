#include "WkCocos/OnlineData/Systems/User.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "Common/App42API.h"

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
				/*
				entityx::ptr<Comp::SaveProfile> sp;
				for (auto entity : entities->entities_with_components(sp))
				{
					if (sp->done)
					{
						entity.remove<Comp::SaveProfile>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!sp->in_progress)
					{
						App42CustomUser user;
						user.userName = sp->m_userid;
						user.customProfile = sp->m_profile_data;
						CCLOG("Saving App42 User %s profile : %s ", user.userName.c_str(), user.customProfile.c_str());
						m_service->createOrUpdateCustomProfile(&user, sp->m_cb);
						sp->in_progress = true;
					}

				}


				entityx::ptr<Comp::LoadProfile> lp;
				for (auto entity : entities->entities_with_components(lp))
				{
					if (lp->done)
					{
						entity.remove<Comp::LoadProfile>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!lp->in_progress)
					{
						CCLOG("Requesting App42 User profile : %s ", lp->m_userid.c_str());
						m_service->GetCustomUser(lp->m_userid.c_str(), lp->m_cb);
						lp->in_progress = true;
					}

				}

				*/

			}


					
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

