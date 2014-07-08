#ifndef __DFGAME_LOADING_ENTITY_COMP_CURLDL_H__
#define __DFGAME_LOADING_ENTITY_COMP_CURLDL_H__

#include "entityx/entityx.h"

#include "curl/curl.h"

namespace WkCocos
{
	namespace Loading
	{
		namespace Comp
		{

			struct LocalFile : entityx::Component<LocalFile> {
				LocalFile(std::string localpath)
				: m_path(localpath)
				{}
				
				std::string getPath()
				{
					return m_path;
				}
				
			private:

				std::string m_path; //local path of the file
			};

			struct LocalMD5 : entityx::Component<LocalMD5> {
				LocalMD5(std::string md5)
				: m_md5(md5)
				{}

				std::string getMD5()
				{
					return m_md5;
				}

			private:

				std::string m_md5; //md5 sig of local file
			};

			struct TempFile : entityx::Component<TempFile> {
				TempFile(std::string localpath)
				: m_path(localpath)
				{}

				std::string getPath()
				{
					return m_path;
				}

			private:

				std::string m_path; //local path of the temp file
			};

			struct TempFileP : entityx::Component<TempFileP> {
				TempFileP(FILE* fp)
				: m_fp(fp)
				{}

				FILE* getFileP()
				{
					return m_fp;
				}

			private:

				FILE* m_fp; //pointer to the temp file
			};

			struct TempMD5 : entityx::Component<TempMD5> {
				TempMD5(std::string md5)
				: m_md5(md5)
				{}

				std::string getMD5()
				{
					return m_md5;
				}

			private:

				std::string m_md5; //md5 sig of temp file
			};


			struct RemoteFile : entityx::Component<RemoteFile> {
				RemoteFile(std::string URL, std::string path)
				: m_url(URL)
				, m_path(path)
				{}

				std::string getURL()
				{
					return m_url;
				}

				std::string getPath()
				{
					return m_path;
				}

			private:

				std::string m_url; //URL of the remote file
				std::string m_path; //path of the remote file
			};

			struct RemoteMD5 : entityx::Component<RemoteMD5> {
				RemoteMD5(std::string md5)
				: m_md5(md5)
				{}

				std::string getMD5()
				{
					return m_md5;
				}

			private:

				std::string m_md5; //md5 sig of remote file
			};

			//Component storing everything needed to Curl easy download
			struct CurlDL : entityx::Component<CurlDL> {
				CurlDL( short retries = 3)
				: m_retries(retries)
				{
				}
				
				char* getErrorBuffer()
				{
					return m_errorbuf;
				}

				unsigned short consumeRetry(unsigned short num = 1)
				{
					m_retries -= num;
					return m_retries;
				}

			private:
				
				char m_errorbuf[CURL_ERROR_SIZE]; //buffer to hold curl error code
				unsigned short m_retries; //number of retries allowed to download this URL
			};

			//TODO : Component for Multi interface
			//Component storing everything needed to Curl easy download
			struct CurlMultiDL : entityx::Component<CurlMultiDL> {
				CurlMultiDL(short retries = 3)
				: m_retries(retries)
				{
				}

				void setFileP(FILE* fp)
				{
					filep = fp;
				}

				FILE* getFileP()
				{
					return filep;
				}

				char* getErrorBuffer()
				{
					return m_errorbuf;
				}

				unsigned short consumeRetry(unsigned short num = 1)
				{
					m_retries -= num;
					return m_retries;
				}

			private:

				FILE* filep;
				char m_errorbuf[CURL_ERROR_SIZE]; //buffer to hold curl error code
				unsigned short m_retries; //number of retries allowed to download this URL
			};

		}//namespace Comp
	}//namespace Loading
}//namespace dfgame

#endif // __DFGAME_LOADING_ENTITY_COMP_CURLDL_H__
