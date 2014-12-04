#ifndef __WKCOCOS_UTILS_LOG_LOGSTREAM_H__
#define __WKCOCOS_UTILS_LOG_LOGSTREAM_H__

#include "WkCocos/Utils/log/nullstream.h"
#include "WkCocos/Utils/log/logstreambuf.h"
#include "WkCocos/Utils/log/loglevel.h"

//to get string << operator for logstream
#include <string>
#include <vector>

#include "WkCocos/Utils/log/logappender.h"

namespace WkCocos
{

	/**
	* \class logstream
	* \brief logstream register a user log and stream it to the buffer.
	*		When the buffer is sync, get a callback to write the log to the different appender.
	*
	*/
	class LogStream : public std::ostream
	{
	private:
		/**
		* default constructor (clog output logstreambuf )
		*/
		LogStream();

	public:
		/**
		* Destructor
		*/
		virtual ~LogStream();

		/**
		* BOUH singleton
		*/
		/**
		* Create a single instance of logger.
		*/
		static void create();

		/**
		* Get the single instance of logger.
		*/
		static LogStream* get();

		/**
		* Destroy the single instance of logger.
		*/
		static void destroy();

		/**
		* add Appender on logger.
		* Class creating the appender is responsible for it's deletion.
		* @param newAppender Appender ptr to add.
		*/
		inline void addAppender(LogAppender* newAppender){ _appenders.push_back(newAppender); }

        /**
        * return the list of appenders
        */
        std::vector<LogAppender*>& getAppenders() { return _appenders; }

		/**
		* Remove an appender
		* Iterate through appenders vector and remove if matching ptr found.
		* @param oldAppender Appender ptr to remove
		*/
		void removeAppender(LogAppender* oldAppender);

		/**
		* Set current level for the stream
		* @param lvl Log level
		*/
		inline void setLevel(loglevel::Level lvl){ m_loglvl = lvl; }

		//manipulator to set *messages's* level
		LogStream & level(loglevel::Level l);
		friend LogStream & operator<<(LogStream  &o, loglevel::Level lvl);

		//set loglevel. Messages logged here will have at least this level
		void resetLevel(loglevel::Level l = loglevel::Core_LogInfo)
		{
			m_loglvl = l;
		}

	private:
		/**
		* Callback to synchronize appenders
		*/
		void syncAppenders();

	private:
		/**
		* Unique instance
		*/
		static LogStream* s_logStream;

		/**
		* Streamed buffer where the log is gonna be written.
		*/
		LogStreamBuf m_buffer;

		/**
		* List of appender to send the log stream to
		*/
		std::vector<LogAppender*> _appenders;

		/**
		* Current Logging level for the stream
		*/
		loglevel::Level			m_loglvl;
	};

} //WkCocos

#ifdef _DEBUG
#	define LOG_DEBUG		(*WkCocos::LogStream::get()) << WkCocos::loglevel::Core_LogDebug
#	define LOG_INFO			(*WkCocos::LogStream::get()) << WkCocos::loglevel::Core_LogInfo
#	define LOG_WARNING		(*WkCocos::LogStream::get()) << WkCocos::loglevel::Core_LogWarning
#	define LOG_ERROR		(*WkCocos::LogStream::get()) << WkCocos::loglevel::Core_LogError
#else //_DEBUG
#	define LOG_DEBUG		std::clog
#	define LOG_INFO			std::clog
#	define LOG_WARNING		std::clog
#	define LOG_ERROR		std::clog
#endif //_DEBUG

#endif // __WKCOCOS_UTILS_LOG_LOGSTREAM_H__
