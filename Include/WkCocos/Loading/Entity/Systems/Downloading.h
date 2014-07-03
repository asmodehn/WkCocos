#ifndef __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__
#define __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__

#include "entityx/entityx.h"

#include "dfgame/Loading/Entity/Comp/DataLoad.h"
#include "dfgame/Loading/Entity/Comp/ProgressValue.h"

#include "dfgame/Utils/Crypto.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <regex>

#include <iostream>
#include <iomanip>
#include <fstream>


#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

#define FILE_DL_EXT ".dlding"

namespace dfgame
{
	namespace Loading
{
		namespace Systems
		{
			//system getting the list of DLC versions available.
			struct DLClisting : public entityx::System<DLClisting>
			{
				DLClisting();

				~DLClisting();

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				void setConnectionTimeout(unsigned int timeout);

				void *_curl;
				unsigned int _connectionTimeout;
				unsigned short _retries;

				std::string m_dirlist;

			};

			//system managing checking which files to download from DLC.
			struct DLCchecking : public entityx::System<DLCchecking>
			{
				DLCchecking();

				~DLCchecking();

				std::vector<unsigned long> splitVersion(std::string vstr);

				void update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt) override;

				void setConnectionTimeout(unsigned int timeout);

				void *_curl;
				unsigned int _connectionTimeout;
				unsigned short _retries;

				std::string m_manifest;

			};

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
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_SYSTEMS_DOWNLOADING_H__
