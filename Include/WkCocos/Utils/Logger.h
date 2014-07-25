#ifndef __WKCOCOS_UTILS_LOGGER_H__
#define __WKCOCOS_UTILS_LOGGER_H__

#include "WkCocos/Utils/log/logstream.h"
#include "WkCocos/Utils/log/logappender.h"

#include <fstream>
//to make sure our operator overload is working with strings
#include <string>
#include <vector>

//TODO
/****
http://www.builderau.com.au/program/java/soa/Use-STL-streams-for-easy-C-thread-safe-logging/0,339024620,320279107,00.htm
http://www.codeproject.com/KB/debug/STLDebugLogger.aspx
http://www.codersource.net/c/c-miscellaneous/overloading-stream-operators.aspx
http://www.google.com/codesearch/p?hl=fr#selboDD9lh0/~janneke/bzr/stream.cc/logstream.hpp&d=3
http://www.drdobbs.com/cpp/201804215;jsessionid=YQFCLX0ATX4L5QE1GHPSKHWATMY32JVN?pgno=3
http://www.codeproject.com/KB/cpp/Encoder.aspx
http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=/com.ibm.xlcpp8a.doc/legacy/ref/s67.htm
http://www.java2s.com/Code/Cpp/Overload/Overloadostreamandistream.htm
http://www.gamedev.net/reference/articles/article2634.asp
http://www.two-sdg.demon.co.uk/curbralan/papers/WritingStreamManipulators.html
Boost Log
*****/



/********************
* Old Logger implementation
*******************/

///just a LOGINDENT value to be defined at build time. this way you have indent levels depending on built libraries
#ifndef LOGINDENTLVL
#define LOGINDENTLVL 0
#endif

///the default tab width for the indent...
#ifndef LOGINDENTWIDTH
#define LOGINDENTWIDTH 2
#endif

///just a LOGPREFIX value to be defined at build time. this way you can have a prefix for the whole project binary (library or executable)
#ifndef LOGPREFIX
#define LOGPREFIX "RAGE Log"
#endif

///To disable logging
#ifndef NRAGELOG
#define RAGELOG
#endif

namespace WkCocos
{

	/**
	* \class Logger
	*
	* \brief This class defines loglevel on top of Logger
	*
	* This Logger use indentation levels, usually defined on build time.
	* TODO : Add LogLevels to filter the console output as well as the file output (3 loglevel : quiet / normal / verbose)
	* Not yet in SDL namespace because it doesnt depend on SDL. However, later it should be made threadsafe, somehow...
	*
	* \author Alex
	*
	* \date 2005/10/05
	*
	* Contact: asmodehn@gna.org
	*
	*/

	typedef enum
	{
		quiet, normal, verbose
	}
	Loglevel;

	class Logger //: public std::ostream
	{
	private:
		/**
		* List of appender to send the log stream to
		*/
		std::vector<LogAppender*> _appenders;

		int _indentlvl, _indentwidth;
		std::string _logprefix;

		bool setLogfile(const std::string & filename);

		static Logger*			_instance;
	public:

		/**
		* Default Constructor that defines how to write the log
		*/
		Logger(const std::string & LogPrefix = LOGPREFIX, int indentlevel = LOGINDENTLVL, int indentwidth = LOGINDENTWIDTH);

		/**
		* Default Destructor that flush the Log Buffer
		*/
		~Logger();

		/**
		* Make singleton for now
		*/
		static void create();
		static Logger& getInstance();
		static void destroy();

		/**
		* add Appender on logger.
		* Class creating the appender is responsible for it's deletion.
		*/
		inline void addAppender(LogAppender* newAppender){ _appenders.push_back(newAppender); }
		void removeAppender(LogAppender* oldAppender)
		{
			auto curAppender = _appenders.begin();
			while (curAppender != _appenders.end() && *curAppender != oldAppender)
			{
				++curAppender;
			}
			if (curAppender != _appenders.end())
			{
				_appenders.erase(curAppender);
			}
		}

		//void add(const std::string & message); // not const because of initial '\n' in string from streams...
		template<typename M>
		Logger& operator << (const M & msg);
		//to enable manipulators on Logger
		Logger& operator << (std::ostream& (*manip)(std::ostream&));
		Logger& operator << (std::ios_base& (*manip)(std::ios_base&));
		Logger& operator << (Logger& (*manip)(Logger&));//to enable specific manipulators on Logger

		friend Logger& nl(Logger& log); // adds a prefix
		friend Logger& endl(Logger& log); // adds a prefix

		Logger & flush(void);

		//TODO : handle operator<< and
		//http://www.mactech.com/articles/mactech/Vol.16/16.04/StreamforAllSeasons/
		//http://www.horstmann.com/cpp/iostreams.html
		//http://spec.winprog.org/streams/

	};

	template<typename M>
	Logger& Logger::operator<< (const M & msg)
	{
#ifdef RAGELOG
		for (auto appender : _appenders)
		{
			//(*appender) << msg;
		}
#endif
		return *this;
	}

	Logger& nl(Logger& log); // adds a new line with the prefix

} // WkCocos


#ifdef _DEBUG
#	define LOG_INFO				WkCocos::Logger::getInstance() /*<< __FILE__ << ":" << __LINE__*/
#	define LOG_WARNING			WkCocos::Logger::getInstance() /*<< __FILE__ << ":" << __LINE__*/
#	define LOG_ERROR			WkCocos::Logger::getInstance() /*<< __FILE__ << ":" << __LINE__*/
#else //_DEBUG
#	define LOG_INFO				std::clog
#	define LOG_WARNING			std::cerr
#	define LOG_ERROR			std::cerr
#endif //_DEBUG

#endif //__WKCOCOS_UTILS_LOGGER_H__