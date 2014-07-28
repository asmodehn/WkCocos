
#ifndef __CRYPTO_H_
#define __CRYPTO_H_

namespace WkCocos
{
	//from https://github.com/dualface/cocos2d-x-extensions
	class Crypto
	{
	public:
		static const int MD5_BUFFER_LENGTH = 16;
		static const int SHA1_BUFFER_LENGTH = 20;

#ifndef CC_UNDER_WIN32

		/** @brief Return AES256 key length */
		static int getAES256KeyLength(void);

		/** @brief Encrypt data with AES256 algorithm, return ciphertext length */
		static int encryptAES256(const void* plaintext,
			int plaintextLength,
			void* ciphertext,
			int ciphertextBufferLength,
			const void* key,
			int keyLength)
		{
			return cryptAES256(false, plaintext, plaintextLength, ciphertext, ciphertextBufferLength, key, keyLength);
		}

		/** @brief Decrypt data with AES256 algorithm, return plaintext length */
		static int decryptAES256(const void* ciphertext,
			int ciphertextLength,
			void* plaintext,
			int plaintextBufferLength,
			const void* key,
			int keyLength)
		{
			return cryptAES256(true, ciphertext, ciphertextLength, plaintext, plaintextBufferLength, key, keyLength);
		}

#endif /* CC_UNDER_WIN32 */

		/** @brief Encoding data with Base64 algorithm, return encoded string length */
		static int encodeBase64(const void* input, int inputLength,
			char* output, int outputBufferLength);


		/** @brief Decoding Base64 string to data, return decoded data length */
		static int decodeBase64(const char* input,
			void* output, int outputBufferLength);

		/** @brief Calculate MD5, get MD5 code (not string) */
		static void MD5(void* input, int inputLength,
			unsigned char* output);

		/** @brief Calculate SHA1 with a secret key. */
		static void sha1(unsigned char* input, int inputLength,
			unsigned char* key, int keyLength,
			unsigned char* buffer, int bufferLength);

	private:
		Crypto(void) {}

		static int cryptAES256(bool isDecrypt,
			const void* input,
			int inputLength,
			void* output,
			int outputBufferLength,
			const void* key,
			int keyLength);

	};

} //namespace dfgame

#endif // __CRYPTO_H_
