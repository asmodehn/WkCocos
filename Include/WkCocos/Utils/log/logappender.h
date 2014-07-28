#ifndef __WKCOCOS_UTILS_LOG_LOGAPPENDER_H__
#define __WKCOCOS_UTILS_LOG_LOGAPPENDER_H__

#include "WkCocos/Utils/log/logstreambuf.h"
#include "WkCocos/Utils/log/loglevel.h"

namespace WkCocos
{
	/**
	* A log appender is basically a log output.
	* When a log is done, the stream will go through all registered appender
	* Inherit this class to define your own appender.
	* Each appender should create their own buffer
	*/
	class LogAppender
	{
	public:
		/**
		* Default constructor
		*/
		LogAppender();

		/**
		* Destructor
		*/
		virtual ~LogAppender();
		
		/**
		*
		*/
		virtual LogAppender& operator << (const LogStreamBuf & msg){ return *this; };

		/**
		* Set level to filter
		*/
		inline void setLevel(loglevel::Level loglvl) { m_loglvl = loglvl; }

		/**
		* Get level filtered
		*/
		inline loglevel::Level  getLevel() { return m_loglvl; }

	protected:
		/**
		* Max level to output
		*/
		loglevel::Level			m_loglvl;
	};

	/**
	* Console appender.
	* Write the stream to the console.
	*/
	class CLogAppender : public LogAppender
	{
	public:
		/**
		* Default constructor
		*/
		CLogAppender();

		/**
		* Destructor
		*/
		virtual ~CLogAppender();

		/**
		* Write in the stream buffer
		*/
		virtual LogAppender& operator << (const LogStreamBuf & msg);
	};

	/**
	* File appender.
	* Write the stream to a file.
	*/
	class FileLogAppender : public LogAppender
	{
	public:
		/**
		* Default constructor
		*/
		FileLogAppender() = delete;

		/**
		* Constructor
		* @param fileName Name of the file to open
		*/
		FileLogAppender(const std::string& fileName);

		/**
		* Destructor
		*/
		virtual ~FileLogAppender();

		/**
		* Write in the stream buffer
		*/
		virtual LogAppender& operator << (const LogStreamBuf & msg);

	private:
		/**
		* File stream to write to
		*/
		std::ofstream _ofstr;
	};

}//namespace WkCocos

#endif //__WKCOCOS_UTILS_LOG_LOGAPPENDER_H__