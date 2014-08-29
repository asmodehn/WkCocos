#ifndef __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__
#define __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__

#include "entityx/entityx.h"

//#include "json/document.h"
//#include "json/filestream.h"

#include "cocos/cocos2d.h"
//#include "WkCocos/Utils/ToolBox.h"

#include <fstream>

namespace WkCocos
{
	namespace LocalData
	{
		namespace Comp
		{
			struct File : entityx::Component<File>
			{
				File(std::string filename)
				: m_filename(filename)
				{}

				~File()
				{
				}

				bool read()
				{
					std::ifstream ifs(cocos2d::FileUtils::getInstance()->fullPathForFilename(m_filename),std::ios::in);

					if (ifs)
					{
						//QUICK file reading ( similar to C code, but still using C++ streams )
						ifs.seekg(0, std::ios::end);
						std::streamoff size = ifs.tellg();

						m_contents.resize(static_cast<std::string::size_type>(size));
						ifs.seekg(0);
						//this doesnt change size of string ( therefore the previous "resize()" call )
						ifs.read(&m_contents[0], size);//WARNING this is supposed to work with C++11 only ( continous string in memory )
						return true;
					}
					else
					{
						return false; // an error happened
					}
				}

				bool write()
				{
					std::ofstream ofs(cocos2d::FileUtils::getInstance()->fullPathForFilename(m_filename), std::ios::out);
					ofs.write(m_contents.c_str(), m_contents.size());

					return true;
				}

				std::string getFilename() const
				{
					return m_filename;
				}

				std::string getContents() const
				{
					return m_contents;
				}

				std::string m_filename;
				std::string m_contents;
			};

			struct LoginID_v1 : entityx::Component<LoginID_v1>
			{
				const std::string m_name = "loginID";

				//warning rapidjson has move semantics implicitly
				LoginID_v1(std::string user, std::string passwd)
				: m_load_cb()
				, m_user(user)
				, m_passwd(passwd)
				{
				}

				LoginID_v1(std::function<void(std::string user, std::string passwd)> load_cb)
					: m_load_cb(load_cb)
				{
				}

				std::string m_user;
				std::string m_passwd;

				std::function<void(std::string user, std::string passwd)> m_load_cb;
			};

			struct PlayerData_v1 : entityx::Component<PlayerData_v1>
			{
				const std::string m_name = "playerData";

				//warning rapidjson has move semantics implicitly
				PlayerData_v1(std::string data)
					: m_load_cb()
					, m_data(data)
				{
				}

				PlayerData_v1(std::function<void(std::string data)> load_cb)
					: m_load_cb(load_cb)
				{
				}

				std::string m_data;

				std::function<void(std::string data)> m_load_cb;
			};


		}//namespace Comp
	}//namespace LocalData
}//namespace WkCocos

#endif // __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__
