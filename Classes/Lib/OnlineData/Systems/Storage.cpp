#include "WkCocos/OnlineData/Systems/Storage.h"

#include "WkCocos/OnlineData/Comp/OnlineData.h"

#include "Common/App42API.h"

namespace WkCocos
{
	namespace OnlineData
	{
		namespace Systems
		{

			Storage::Storage()
			{
				m_service = App42API::BuildStorageService();
			
			}

			Storage::~Storage()
			{
				//StorageService::Terminate();
				//m_service = nullptr;
			}

			void Storage::update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt)
			{/*
				entityx::ptr<Comp::LoadProfile> l;
				for (auto entity : entities->entities_with_components(l))
				{
					if (l->done)
					{
						entity.remove<Comp::Create>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!l->in_progress)
					{
						CCLOG("Loading data Id : %s ", l->m_userid.c_str());
						m_service->FindDocumentById(l->m_userid.c_str(), l->m_passwd.c_str(), l->m_email.c_str(), c->m_cb);
						l->in_progress = true;
					}
				}

				entityx::ptr<Comp::Save> s;
				for (auto entity : entities->entities_with_components(s))
				{
					if (s->done)
					{
						entity.remove<Comp::Login>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!s->in_progress)
					{
						CCLOG("Saving data Id : %s ", s->m_userid.c_str());
						m_service->InsertJsonDocument(s->m_userid.c_str(), s->m_passwd.c_str(), s->m_cb);
						s->in_progress = true;
					}
					
				}

				entityx::ptr<Comp::PublicLoad> pl;
				for (auto entity : entities->entities_with_components(pl))
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
					else if (!pl->in_progress)
					{
						CCLOG("Loading data Id : %s ", pl->m_userid.c_str());
						m_service->FindDocumentById(pl->m_userid.c_str(), pl->m_passwd.c_str(), pl->m_email.c_str(), pl->m_cb);
						pl->in_progress = true;
					}
				}

				entityx::ptr<Comp::Save> ps;
				for (auto entity : entities->entities_with_components(ps))
				{
					if (ps->done)
					{
						entity.remove<Comp::Login>();
						//if mask at 0 no request in this entity anymore
						if (entity.component_mask() == 0)
						{
							entity.destroy();
						}
					}
					else if (!ps->in_progress)
					{
						CCLOG("Saving data Id : %s ", ps->m_userid.c_str());
						m_service->InsertJsonDocument(ps->m_userid.c_str(), ps->m_passwd.c_str(), ps->m_cb);
						ps->in_progress = true;
					}

				}
				*/
			}


					
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

