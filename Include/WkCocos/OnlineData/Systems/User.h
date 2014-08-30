#ifndef __DFGAME_ONLINEDATA_SYSTEMS_USER_H__
#define __DFGAME_ONLINEDATA_SYSTEMS_USER_H__

#include "entityx/entityx.h"

#include "Common/App42API.h"

namespace WkCocos 
{
	namespace OnlineData
	{
		namespace Systems
		{
			//system finding error and launching error behavior
			struct User : public entityx::System<User>
			{
				User();
				~User();
				void update(entityx::ptr<entityx::EntityManager> entities, entityx::ptr<entityx::EventManager> events, double dt) override;

			protected :
				::App42::UserService* m_user_service;
			};
		}//namespace Systems
	}//namespace OnlineData
}//namespace WkCocos

#endif // __DFGAME_ONLINEDATA_SYSTEMS_LOGIN_H__
