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

			}


					
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

