#include "WkCocos/StrongBox/StrongBox.h"

#include <sstream>
#include <iomanip>

namespace WkCocos
{
	namespace StrongBox
	{
		//internal method
		void StrongBox::copy_key(unsigned char * key, size_t key_len)
		{
			m_key_len = key_len;
			m_key = (unsigned char*)malloc(sizeof(unsigned char) * m_key_len);
			memcpy(m_key, key, m_key_len);
		}

		void StrongBox::move_key(unsigned char *& key, size_t& key_len)
		{
			m_key_len = key_len;
			m_key = key;

			key = nullptr;
			key_len = 0;
		}

		void StrongBox::copy_value(unsigned char * value, size_t val_len)
		{
			m_value_len = val_len;
			m_value = (unsigned char*)malloc(sizeof(unsigned char) * m_value_len);
			memcpy(m_value, value, m_value_len);
		}

		void StrongBox::move_value(unsigned char *& value, size_t& val_len)
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
			//we truncate/fill the key here, to be safe.
			char keyc[17];
			strncpy(keyc, key.c_str(), 16);
			keyc[16] = '\0';
			copy_key(reinterpret_cast<unsigned char*>(keyc), 16);
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
		void StrongBox::set(std::string value)
		{
			xxtea_long data_len = sizeof(value.c_str());
			unsigned char* data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * data_len));
			strncpy(reinterpret_cast<char*>(data), value.c_str(), data_len); //seeing value as a pack of bytes

			xxtea_long ret_len;
			//this will malloc.
			m_value = xxtea_encrypt(data, data_len, m_key, m_key_len, &ret_len);
			m_value_len = ret_len;

		}

		template<>
		std::string StrongBox::get()
		{
			xxtea_long ret_len;
			unsigned char* data = xxtea_decrypt(m_value, m_value_len, m_key, m_key_len, &ret_len);

			return std::string(reinterpret_cast<char*>(data));
		}

		void StrongBox::set_encrypted(std::string encrypted_value)
		{
			//removing existing value
			if (m_value_len > 0)
			{
				free(m_value);
			}

			//reading hex representation and storing in m_value
			std::stringstream ss;
			
			xxtea_long m_value_len = strlen(encrypted_value.c_str());
			//note : if string is empty, m_value_len is 0 but malloc can return a null pointer OR "the behavior is as if the size were some nonzero value, except that the returned pointer shall not be used to access an object"
			//so we need the if here to make sure if str is empty we force a behavior
			if (m_value_len > 0)
			{
				
				m_value = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * m_value_len));
				if (m_value)
				{
					unsigned short buffer = 0;
					size_t buffer_size = sizeof(buffer);//should be 2
					int offset = 0;
					while (offset*buffer_size < m_value_len) {
						ss.clear();
						ss << std::hex << encrypted_value.substr(offset*buffer_size, buffer_size);
						ss >> buffer;
						m_value[offset] = static_cast<unsigned char>(buffer);
						++offset;
					}
				}
			}
		}

		std::string StrongBox::get_encrypted()
		{
			//reading m_value and converting to hex representation
			std::stringstream ss;
			ss << std::hex << std::setfill('0');
			for (unsigned short i = 0; i < m_value_len; ++i)
			{
				ss << std::setw(sizeof(i)) << static_cast<unsigned>(m_value[i]);
			}
			return ss.str();
		}
	}
}