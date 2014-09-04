#ifndef __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__
#define __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__

#include "entityx/entityx.h"

//#include "json/document.h"
//#include "json/filestream.h"

#include "cocos/cocos2d.h"
#include "WkCocos/StrongBox/StrongBox.h"
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
				File()
					: m_filename("")
				, m_remove(false)
				{}

				File(std::string filename, std::string key = "", bool remove = false)
				: m_filename(filename)
				, m_remove(remove)
				, m_sbox(key)
				{
				}


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

						size_t size = static_cast<size_t>(ifs.tellg());

						std::string contents;
						contents.resize(size);

						ifs.seekg(0);
						//this doesnt change size of string ( therefore the previous "resize()" call )
						ifs.read(&contents[0], size);//WARNING this is supposed to work with C++11 only ( continous string in memory )
						
						if (m_sbox.isEncrypted())
						{
							//directly store in strongbox
							m_sbox.set_encryptedHex(contents);
						}
						else
						{
							m_sbox.set<std::string>(contents);
						}

						return true;
					}
					else
					{
						return false; // an error happened
					}
				}

				bool write()
				{
					if (m_remove)
					{
						std::remove(cocos2d::FileUtils::getInstance()->fullPathForFilename(m_filename).c_str());
					}
					else
					{
						std::string str;
						if (m_sbox.isEncrypted())
						{
							str = m_sbox.get_encryptedHex();
						}
						else
						{
							str = m_sbox.get<std::string>();
						}

						std::ofstream ofs(cocos2d::FileUtils::getInstance()->fullPathForFilename(m_filename), std::ios::out);
						ofs.write(str.c_str(), str.size());
					}

					return true;
				}

				std::string getFilename() const
				{
					return m_filename;
				}

				std::string getContents() const
				{
					return m_sbox.get<std::string>();
				}

				void setContents(std::string str)
				{
					return m_sbox.set<std::string>(str);
				}

			private:
				std::string m_filename;
				WkCocos::StrongBox::StrongBox m_sbox;
				bool		m_remove;
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

			struct Read : entityx::Component<Read>
			{
				Read(){}

			};


		}//namespace Comp
	}//namespace LocalData
}//namespace WkCocos

#endif // __WKCOCOS_LOCALDATA_COMP_LOCALDATA_H__
