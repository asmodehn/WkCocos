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
		* If the key is empty string, no encryption is done but everything should work as expected.
		*/
		class StrongBox
		{
		private:
			//key here to not be too close to value in memory
			unsigned char * m_key;
			xxtea_long m_key_len;

			//internal method
			void copy_key(unsigned char * key, xxtea_long key_len);
			void move_key(unsigned char *& key, xxtea_long& key_len);
			void copy_value(unsigned char * value, xxtea_long val_len);
			void move_value(unsigned char *& value, xxtea_long& val_len);
		public :

			//exposing our conversion function from bin to string.
			//this will malloc the C string
			static void hexString2BinVal(const std::string&, unsigned char *& val, xxtea_long & val_length);
			static void binVal2HexString(unsigned char * val, xxtea_long val_length, std::string&);

			/*
			* Constructor with key argument.
			* @param key : encryption key used by the cipher. The key is a 16 bytes(128 bits) string
			* User should provide their own key and not rely on the default.
			*/
			StrongBox(const std::string& key = "");

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
			T get() const;

			/*
			* check if encryption is enabled
			*/
			inline bool isEncrypted() const
			{
				return (m_key_len > 0);
			}

			/*
			* Accessor that directly get the encrypted value in the strong box as string
			* @return the string
			*/
			std::string get_encryptedHex() const;

			/*
			* Accessor that directly set the encrypted value in the strong box as string
			* @param encrypted_value : the hex string with encrypted value
			*/
			void set_encryptedHex(std::string encrypted_value);

		private:
			unsigned char * m_value;
			xxtea_long m_value_len;
		};


////////generic code for get and set////////
////////WARNING : THIS IS NOT PORTABLE CODE
////////TODO : make it portable

		template<typename T>
		void StrongBox::set(T value)
		{
			//removing existing value
			if (m_value_len > 0)
			{
				free(m_value);
			}

			xxtea_long data_len = sizeof(value);//POD types only
			unsigned char* data = reinterpret_cast<unsigned char*>(&value); //seeing value as a pack of bytes

			if (isEncrypted())
			{
				xxtea_long ret_len;
				//this will malloc.
				m_value = xxtea_encrypt(data, data_len, m_key, m_key_len, &ret_len);
				m_value_len = ret_len;
			}
			else
			{
				//this will malloc.
				copy_value(data, data_len);
			}

		}

		template<typename T>
		T StrongBox::get() const
		{
			xxtea_long ret_len;
			unsigned char* data;
			T result;
			if (isEncrypted())
			{
				//this will malloc
				data = xxtea_decrypt(m_value, m_value_len, m_key, m_key_len, &ret_len);
				//data == 0 means there is a problem
				result = *(reinterpret_cast<T*>(data));
				free(data);//destroying memory after copy
			}
			else
			{
				data = m_value;
				ret_len = m_value_len;
				result = *(reinterpret_cast<T*>(data));
			}
			//copy on return
			return result;
		}

////////specialization for getters and setters if needed///////////////


template<>
void StrongBox::set<std::string>(std::string value);

template<>
std::string StrongBox::get<std::string>() const;


	}//namespace StrongBox
}//namespace WkCocos



#endif // __WKCOCOS_STRONGBOX_STRONGBOX_H__
