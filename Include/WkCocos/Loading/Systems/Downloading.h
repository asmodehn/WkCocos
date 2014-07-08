#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__

#include "entityx/entityx.h"

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

#define FILE_DL_EXT ".dlding"

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{

			//system managing downloading. N at a time.
			struct Downloading : public entityx::System<Downloading>
			{
				Downloading();

				~Downloading();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				void setConnectionTimeout(unsigned int timeout);

				/**
				* Computes the md5 signature of the file at the filepath.
				* returns the md5 signature as hex string or an empty string if an error happened
				*/
				std::string computeMD5(std::string filepath);

				void *_curl;
				unsigned int _connectionTimeout;
				unsigned short _retries;

				std::string m_dirlist;

			};

		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__
