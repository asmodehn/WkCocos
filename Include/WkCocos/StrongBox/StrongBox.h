#ifndef __WKCOCOS_STRONGBOX_STRONGBOX_H__
#define __WKCOCOS_STRONGBOX_STRONGBOX_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "xxtea/xxtea.h"

#include <bitset>

namespace WkCocos
{
	namespace StrongBox
	{
		/*
		* This class implements a basic (weak) memory encryption of data.
		* Only XXTEA is available now, but more ciphers can be added later (AES?)
		* This is not really safe as the key is under the mat.
		*/
		class StrongBox
		{
		private:
			//key here to not be too close to value in memory
			unsigned char * m_key;
			xxtea_long m_key_len;

			//internal method
			void copy_key(unsigned char * key, size_t key_len);
			void move_key(unsigned char *& key, size_t& key_len);
			void copy_value(unsigned char * value, size_t val_len);
			void move_value(unsigned char *& value, size_t& val_len);
		public :

			/*
			* Constructor with key argument.
			* @param key : encryption key used by the cipher. The key is a 16 bytes(128 bits) string
			* User should provide their own key and not rely on the default.
			*/
			StrongBox(const std::string& key = "53cr3t");

			/*
			* Constructor with value and default key.
			* @param value : value to store in the strongbox.
			* @param key : encryption key used by the cipher. The key is a 16 bytes(128 bits) string
			* User should provide their own key and not rely on the default.
			*/
			template<typename T>
			StrongBox(const T & value, const std::string& key = "53cr3t");

			/*
			* Copy Constructor
			*/
			StrongBox(const StrongBox& sb);

			/*
			* copy Assignment Operator
			*/
			StrongBox& operator=(const StrongBox& sb);

			/*
			* Move Constructor
			*/
			StrongBox(StrongBox&& sb);

			/*
			* Move Assignment Operator
			*/
			StrongBox& operator=(StrongBox&& sb);

			/*
			* Destructor
			*/
			~StrongBox();

			/*
			* Accessor that set the value in the strong box
			*/
			template<typename T>
			void set(T value);

			/*
			* Accessor that get the value in the strong box
			*/
			template<typename T>
			T get();

			/*
			* Accessor that directly get the encrypted value in the strong box as string
			* the string
			*/
			std::string get_encrypted();

			/*
			* Accessor that directly set the encrypted value in the strong box as string
			* param encrypted_value : the hex string with encrypted value
			*/
			void set_encrypted(std::string encrypted_value);

		private:
			unsigned char * m_value;
			xxtea_long m_value_len;
		};


////////generic code for get and set////////
////////WARNING : THIS IS NOT PORTABLE CODE
////////TODO : make it portable

		template<typename T>
		StrongBox::StrongBox(const T & value, const std::string& key)
			: m_value_len(0)
			, m_key_len(0)
		{
			//we truncate/fill the key here, to be safe.
			char keyc[17];
			strncpy(keyc, key.c_str(), 16);
			keyc[16] = '\0';
			copy_key(reinterpret_cast<unsigned char*>(keyc), 16);
			set<T>(value);
		}

		template<typename T>
		void StrongBox::set(T value)
		{
			xxtea_long data_len = sizeof(value);
			unsigned char* data = reinterpret_cast<unsigned char*>(&value); //seeing value as a pack of bytes

			xxtea_long ret_len;
			//this will malloc.
			m_value = xxtea_encrypt(data, data_len, m_key, m_key_len, &ret_len);
			m_value_len = ret_len;

		}

		template<typename T>
		T StrongBox::get()
		{
			xxtea_long ret_len;
			unsigned char* data = xxtea_decrypt(m_value, m_value_len, m_key, m_key_len, &ret_len);

			return *(reinterpret_cast<T*>(data));
		}

////////specialization for getters and setters if needed///////////////


template<>
void StrongBox::set(std::string value);

template<>
std::string StrongBox::get();


	}//namespace StrongBox
}//namespace WkCocos



#endif // __WKCOCOS_STRONGBOX_STRONGBOX_H__