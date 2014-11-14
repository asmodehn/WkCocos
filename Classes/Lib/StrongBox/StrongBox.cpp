#include "WkCocos/StrongBox/StrongBox.h"

#include <sstream>
#include <iomanip>

namespace WkCocos
{
	namespace StrongBox
	{
		//internal method
		void StrongBox::copy_key(unsigned char * key, xxtea_long key_len)
		{
			m_key_len = key_len;
			if (m_key_len == 0) //to counter non deterministic malloc behavior when allocation 0 bytes
			{
				m_key = 0;
			}
			else
			{
				m_key = (unsigned char*)malloc(sizeof(unsigned char) * m_key_len);
				memcpy(m_key, key, m_key_len);
			}
		}

		void StrongBox::move_key(unsigned char *& key, xxtea_long& key_len)
		{
			m_key_len = key_len;
			m_key = key;

			key = nullptr;
			key_len = 0;
		}

		void StrongBox::copy_value(unsigned char * value, xxtea_long val_len)
		{
			m_value_len = val_len;
			if (m_value_len == 0) //to counter non deterministic malloc behavior when allocation 0 bytes
			{
				m_value = 0;
			}
			else
			{
				m_value = (unsigned char*)malloc(sizeof(unsigned char) * m_value_len);
				memcpy(m_value, value, m_value_len);
			}
		}

		void StrongBox::move_value(unsigned char *& value, xxtea_long& val_len)
		{
			m_value_len = val_len;
			m_value = value;

			value = nullptr;
			val_len = 0;
		}

		StrongBox::StrongBox(const std::string& key)
			: m_value_len(0)
			, m_value(nullptr)
			, m_key_len(0)
			, m_key(nullptr)
		{
			if (key.length() > 0)
			{
				//we truncate/fill the key here, to be safe.
				char keyc[17];
				strncpy(keyc, key.c_str(), 16);
				keyc[16] = '\0';
				copy_key(reinterpret_cast<unsigned char*>(keyc), 16);
			}
		}
		
		StrongBox::StrongBox(const StrongBox& sb)
			: m_value_len(0)
			, m_value(nullptr)
			, m_key_len(0)
			, m_key(nullptr)
		{
			copy_key(sb.m_key, sb.m_key_len);
			copy_value(sb.m_value, sb.m_value_len);
		}

		StrongBox& StrongBox::operator=(const StrongBox& sb)
		{
			copy_key(sb.m_key, sb.m_key_len);
			copy_value(sb.m_value, sb.m_value_len);
			return *this;
		}

		/*
		* Move Constructor
		*/
		StrongBox::StrongBox(StrongBox&& sb)
			: m_value_len(0)
			, m_value(nullptr)
			, m_key_len(0)
			, m_key(nullptr)
		{
			move_key(sb.m_key, sb.m_key_len);
			move_value(sb.m_value, sb.m_value_len);
		}

		/*
		* Move Assignment Operator
		*/
		StrongBox& StrongBox::operator=(StrongBox&& sb)
		{
			if (this != &sb)
			{
				if (m_key_len > 0)
				{
					free(m_key);
				}
				if (m_value_len > 0)
				{
					free(m_value);
				}
				move_key(sb.m_key, sb.m_key_len);
				move_value(sb.m_value, sb.m_value_len);
			}
			return *this;
		}

		StrongBox::~StrongBox()
		{
			if (m_key_len > 0)
			{
				free(m_key);
			}
			if (m_value_len > 0)
			{
				free(m_value);
			}
		}

		//specialized getters and setters
		template<>
		void StrongBox::set<std::string>(std::string value)
		{
			//removing existing value
			if (m_value_len > 0)
			{
				free(m_value);
			}

			xxtea_long data_len = strlen(value.c_str());
			unsigned char* data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * data_len +1));
			strncpy(reinterpret_cast<char*>(data), value.c_str(), data_len ); //seeing value as a pack of bytes
			data[data_len] = '\0'; //forcing end of string.
			data_len++;//including \0 in the buffer size

			if (isEncrypted())
			{
				xxtea_long ret_len;
				//this will malloc.
				m_value = xxtea_encrypt(data, data_len, m_key, m_key_len, &ret_len);
				m_value_len = ret_len;
				free(data);
			}
			else
			{
				//we already have malloc data. we should move instead of copying.
				move_value(data, data_len);
			}
		}

		template<>
		std::string StrongBox::get<std::string>() const
		{
			xxtea_long ret_len;
			unsigned char* data;
			std::string result;
			if (isEncrypted())
			{
				//this will malloc
				data = xxtea_decrypt(m_value, m_value_len, m_key, m_key_len, &ret_len);
				result = std::string(reinterpret_cast<char*>(data), ret_len - 1);
				free(data);
			}
			else
			{
				data = m_value;
				ret_len = m_value_len;
				result = std::string(reinterpret_cast<char*>(data), ret_len - 1);
			}
			//copy on string construct
			//be careful with the \0 added at the end of a buffer comming from a string.
			return result;
		}

		void StrongBox::hexString2BinVal(const std::string&  hex, unsigned char *& val, xxtea_long & val_length)
		{
			//reading hex representation and storing in m_value
			std::stringstream ss;

			//sizeof : 2 characters to represent size of 1 byte.
			val_length = strlen(hex.c_str()) / 2;
			//note : if string is empty, m_value_len is 0 but malloc can return a null pointer OR "the behavior is as if the size were some nonzero value, except that the returned pointer shall not be used to access an object"
			//so we need the if here to make sure if str is empty we force a behavior
			if (val_length > 0)
			{
				val = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * val_length +1));
				if (val)
				{
					unsigned short buffer = 0;
					size_t buffer_size = sizeof(buffer);//should be 2
					size_t offset = 0;
					while (offset < val_length) {
						ss.clear();
						ss << std::hex << hex.substr(offset*buffer_size, buffer_size);
						ss >> buffer;
						val[offset] = static_cast<unsigned char>(buffer);
						++offset;
					}
					//forcing '\0' at the end
					val[val_length] = '\0';
				}
			}
		}

		void StrongBox::binVal2HexString(unsigned char * val, xxtea_long val_length, std::string& hex)
		{
			//reading m_value and converting to hex representation
			std::stringstream ss;
			ss << std::hex << std::setfill('0');
			for (unsigned short i = 0; i < val_length; ++i)
			{
				ss << std::setw(sizeof(i)) << static_cast<unsigned>(val[i]);
			}

			hex = ss.str();
		}

		void StrongBox::set_encryptedHex(std::string encrypted_value)
		{
			//removing existing value
			if (m_value_len > 0)
			{
				free(m_value);
			}

			hexString2BinVal(encrypted_value, m_value, m_value_len);
		}

		std::string StrongBox::get_encryptedHex() const
		{
			std::string hexresult;
			binVal2HexString(m_value, m_value_len, hexresult);
			return hexresult;
		}
	} //namespace StrongBox
}//namespace WkCocos
