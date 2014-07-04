#ifndef __WKCOCOS_UTILS_LOG_LOGSTREAM_H__
#define __WKCOCOS_UTILS_LOG_LOGSTREAM_H__

#include "WkCocos/Utils/log/nullstream.h"
#include "WkCocos/Utils/log/logstreambuf.h"
#include "WkCocos/Utils/log/loglevel.h"

//to get string << operator for logstream
#include <string>

namespace WkCocos
{

	/**
	* \class logstream
	* \brief logstream provide filtering based on loglevel.
	*
	*/
	///@TODO : Design should be changed as we cannot access stringbuf inside an ostringstream in a standard / portable way...
	class LogStream : public std::ostringstream
	{
		//streambuff
		LogStreamBuf* pvm_lsb;

		//level of hte log stream.
		//anything less important than this level is ignored
		loglevel::Level loglvl;

	public:
		//default constructor (clog output logstreambuf )
		LogStream();
		explicit LogStream(LogStreamBuf* lsb);
		~LogStream();

		//to manage prefix
		void resetprefix(const std::string & newprefix = "");
		const std::string& getprefix() const;

		//to use logstream as streamthrough
		friend std::ostream& operator<<(std::ostream& o, LogStream & l)
		{
			return o << l.rdbuf();
		};

		LogStreamBuf* rdbuf() const
		{
			return pvm_lsb;
		}
		//not needed anymore since we inherit from ostringstream
		//std::string str ( ) const { return rdbuf()->str(); }
		//void str ( const std::string & s ) {return rdbuf()->str(s); }

		///Unformatted output
		///Put character (public member function)
		//logstream& put ( char c );
		///Write block of data (public member function)
		//logstream& write ( const char* s , std::streamsize n );

		///Synchronization
		///Flush output stream buffer (public member function)
		///flush will go to std::clog
		//logstream& flush ( );

		//set loglevel. Messages logged here will have at least this level
		void resetLevel(loglevel::Level l = loglevel::Core_LogInfo)
		{
			loglvl = l;
		}
		loglevel::Level getLevel()
		{
			return loglvl;
		}

		//manipulator to set *messages's* level
		friend LogStream & operator<<(LogStream  &o, loglevel::Level lvl);
		LogStream & level(loglevel::Level l);

	};

} //WkCocos



#endif // __WKCOCOS_UTILS_LOG_LOGSTREAM_H__