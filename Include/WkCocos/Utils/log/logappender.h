#ifndef __WKCOCOS_UTILS_LOG_LOGAPPENDER_H__
#define __WKCOCOS_UTILS_LOG_LOGAPPENDER_H__

#include "WkCocos/Utils/log/logstream.h"

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
		* Write in the stream buffer
		*/
		template<typename M>
		LogAppender& operator << (const M & msg);

		/**
		*
		*/
		virtual LogAppender& operator << (const LogStream & msg){ return *this; };

		virtual void onOverflow(int c);
	protected:
		/**
		* Stream where the log goes through
		* Stream should be in logger, not appender, cause anyway the same string wont give two different result.
		* It's here because buffer behave differently based on appender.
		* Also, buffer can be read only once, so it need to be duplicate.
		* Not the best design, but we need something that will work for now
		*/
		LogStream	_stream;
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
		virtual LogAppender& operator << (const LogStream & msg);
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
		virtual LogAppender& operator << (const LogStream & msg);

	private:
		/**
		* File stream to write to
		*/
		std::ofstream _ofstr;
	};

	template<typename M>
	LogAppender& LogAppender::operator << (const M & msg)
	{
		_stream << msg;
		*this << _stream;
		return *this;
	}

}//namespace WkCocos

#endif //__WKCOCOS_UTILS_LOG_LOGAPPENDER_H__