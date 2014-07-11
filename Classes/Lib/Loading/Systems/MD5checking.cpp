#include "WkCocos/Loading/Systems/MD5checking.h"

#include "WkCocos/Loading/Events/Error.h"

#include "WkCocos/Utils/Crypto.h"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace WkCocos
{
	namespace Loading
	{
		namespace Systems
		{

			std::string MD5checking::computeMD5(std::string filepath)
			{
				std::string current_md5 = "";

				//CCLOG(" CHECKING MD5 for %s", filepath.c_str());
				cocos2d::Data filedata = cocos2d::FileUtils::getInstance()->getDataFromFile(filepath.c_str());

				if (!filedata.isNull()) //read successful
				{
					unsigned char hashbuffer[Crypto::MD5_BUFFER_LENGTH];
					Crypto::MD5((void*)filedata.getBytes(), filedata.getSize(), hashbuffer);
					//converting to string with hex
					std::stringstream ss;
					ss << std::hex << std::setfill('0');
					for (int i = 0; i < sizeof(hashbuffer); ++i)
					{
						ss << std::setw(2) << static_cast<unsigned int>(hashbuffer[i]);
					}
					current_md5 = ss.str();
					//std::cout << filepath << " : " << current_md5 << std::endl;
				}
				return current_md5;
			}

			void MD5checking::update(entityx::ptr<entityx::EntityManager> es, entityx::ptr<entityx::EventManager> events, double dt)
			{
				entityx::ptr<Comp::LocalFile> localfile;
				for (auto entity : es->entities_with_components(localfile))
				{
					if (!entity.component<Comp::LocalMD5>())
					{
						std::string sig = computeMD5(localfile->getPath());

						//registering the md5 signature
						entity.assign<Comp::LocalMD5>(sig);

						//one at a time is good enough, we need to keep update looping fast.
						break;
					}
				}

				entityx::ptr<Comp::TempFile> tmpfile;
				for (auto entity : es->entities_with_components(tmpfile))
				{
					if (entity.component<Comp::CurlDL>())
					{ //download is still on going, dont check yet
						continue;
					}
					else if (!entity.component<Comp::TempMD5>())
					{
						std::string sig = computeMD5(tmpfile->getPath());

						//registering the md5 signature
						entity.assign<Comp::TempMD5>(sig);

						//one at a time is good enough, we need to keep update looping fast.
						break;
					}
				}

			};
					
		}//namespace Systems
	}//namespace Loading
}//namespace WkCocos

