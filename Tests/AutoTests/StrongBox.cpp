// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/StrongBox/StrongBox.h"

#include <string>

SUITE(StrongBox)
{
	struct EncryptedStrongBox
	{
		EncryptedStrongBox() : m_sbox("myVerySecretKey") { /* some setup */ }
		~EncryptedStrongBox() { /* some teardown */ }

		WkCocos::StrongBox::StrongBox m_sbox;
	};

	struct ClearStrongBox
	{
		ClearStrongBox() { /* some setup */ } //default StrongBox constructor must construct a clear strongbox
		~ClearStrongBox() { /* some teardown */ }

		WkCocos::StrongBox::StrongBox m_sbox;
	};

	struct DoubleEncryptedStrongBox : public EncryptedStrongBox
	{
		DoubleEncryptedStrongBox() : m_sbox_bis(m_sbox) { /* some setup */ }
		~DoubleEncryptedStrongBox() { /* some teardown */ }

		WkCocos::StrongBox::StrongBox m_sbox_bis;
	};

	struct DoubleClearStrongBox : public ClearStrongBox
	{
		DoubleClearStrongBox() : m_sbox_bis(m_sbox){ /* some setup */ }
		~DoubleClearStrongBox() { /* some teardown */ }

		WkCocos::StrongBox::StrongBox m_sbox_bis;
	};

	//Testing if encryption and decryption are symmetric for int
	TEST_FIXTURE(ClearStrongBox, ClearSymmetric_Int)
	{
		int original_value = 42;

		m_sbox.set(original_value);

		int decoded_value = m_sbox.get<int>();

		CHECK(original_value == decoded_value);

	}

	//Testing if encryption and decryption are symmetric for string
	TEST_FIXTURE(ClearStrongBox, ClearSymmetric_String)
	{
		std::string original_value = "fortytwo";

		m_sbox.set(original_value);

		std::string decoded_value = m_sbox.get<std::string>();

		CHECK(original_value == decoded_value);
	}

	//Testing if clear encryption doesnt change the value for int in our case
	TEST_FIXTURE(ClearStrongBox, ClearNoEncrypt_Int)
	{
		int raw_value = 42;

		m_sbox.set(raw_value);

		//ENDIANNESS is not checked. value might change...
		//TOFIX
		std::string enc_str = m_sbox.get_encryptedHex();
		unsigned char* enc_buffer;
		xxtea_long enc_buffer_len;
		WkCocos::StrongBox::StrongBox::hexString2BinVal(enc_str, enc_buffer, enc_buffer_len);
		int enc_value = *(reinterpret_cast<int*>(enc_buffer));//this will copy the int
		free(enc_buffer);
		CHECK(raw_value == enc_value);
	}

	//Testing if clear encryption doesnt change the value for string in our case
	TEST_FIXTURE(ClearStrongBox, ClearNoEncrypt_String)
	{
		std::string raw_value = "fortytwo";

		m_sbox.set(raw_value);

		std::string enc_str = m_sbox.get_encryptedHex();
		unsigned char* enc_buffer;
		xxtea_long enc_buffer_len;
		WkCocos::StrongBox::StrongBox::hexString2BinVal(enc_str, enc_buffer, enc_buffer_len);
		std::string enc_value(reinterpret_cast<char*>(enc_buffer));//this will copy the string
		free(enc_buffer);
		CHECK(raw_value == enc_value);
	}

	//Testing if encryption and decryption are symmetric for int
	TEST_FIXTURE(EncryptedStrongBox, EncryptedSymmetric_Int)
	{
		int original_value = 42;

		m_sbox.set(original_value);

		int decoded_value = m_sbox.get<int>();

		CHECK(original_value == decoded_value);
	}

	//Testing if encryption and decryption are symmetric for string
	TEST_FIXTURE(EncryptedStrongBox, EncryptedSymmetric_String)
	{
		std::string original_value = "fortytwo";

		m_sbox.set(original_value);

		std::string decoded_value = m_sbox.get<std::string>();
		
		CHECK(original_value == decoded_value);
	}

	//Testing if encryption change the value for int in our case
	TEST_FIXTURE(EncryptedStrongBox, EncryptedEncrypt_Int)
	{
		int raw_value = 42;

		m_sbox.set(raw_value);

		std::string enc_str = m_sbox.get_encryptedHex();
		unsigned char* enc_buffer;
		xxtea_long enc_buffer_len;
		WkCocos::StrongBox::StrongBox::hexString2BinVal(enc_str, enc_buffer, enc_buffer_len);
		int enc_value = *(reinterpret_cast<int*>(enc_buffer));//this will copy the int
		free(enc_buffer);
		CHECK(raw_value != enc_value);
	}

	//Testing if encryption change the value for string in our case
	TEST_FIXTURE(EncryptedStrongBox, EncryptedEncrypt_String)
	{
		std::string raw_value = "fortytwo";

		m_sbox.set(raw_value);

		std::string enc_str = m_sbox.get_encryptedHex();
		unsigned char* enc_buffer;
		xxtea_long enc_buffer_len;
		WkCocos::StrongBox::StrongBox::hexString2BinVal(enc_str, enc_buffer, enc_buffer_len);
		std::string enc_value(reinterpret_cast<char*>(enc_buffer)); //this will copy the string
		free(enc_buffer);
		CHECK(raw_value != enc_value);
	}



	//Testing if direct encrypted value accessor is symmetric for int
	TEST_FIXTURE(DoubleClearStrongBox, ClearEncryptAccessSymmetric_Int)
	{
		int raw_value = 42;

		m_sbox.set(raw_value);

		std::string enc_str = m_sbox.get_encryptedHex();

		m_sbox_bis.set_encryptedHex(enc_str);

		int enc_value = m_sbox_bis.get<int>();

		CHECK(raw_value == enc_value);
	}
	TEST_FIXTURE(DoubleEncryptedStrongBox, EncryptedEncryptAccessSymmetric_Int)
	{
		int raw_value = 42;

		m_sbox.set(raw_value);

		std::string enc_str = m_sbox.get_encryptedHex();

		m_sbox_bis.set_encryptedHex(enc_str);

		int enc_value = m_sbox_bis.get<int>();

		CHECK(raw_value == enc_value);
	}

	//Testing if direct encrypted value accessor is symmetric for string
	//TODO

}
