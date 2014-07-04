#include "WkCocos/Utils/log/logstreambuf.h"

#include <stdexcept>
#include <cstring>

namespace WkCocos
{

	LogStreamBuf::LogStreamBuf()
		: std::stringbuf(),//stringbuf in and out required
		ptm_prefix(""), //default no prefix
		ptm_logtime(true),
		ptm_filterin(true)

	{
		}

	LogStreamBuf::~LogStreamBuf()
	{
	}


	void LogStreamBuf::resetprefix(const std::string& newprefix)
	{
		ptm_prefix = newprefix;
	}

	const std::string& LogStreamBuf::getprefix() const
	{
		return ptm_prefix;
	}

	std::string LogStreamBuf::getlocaltime()
	{
		//TODO : insert useful prefixes
		//maybe distinction date / time useless
		char timebuf[32];
		if (ptm_logtime)
		{
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime); //TODO Windows : localtime_s
			//TOFIX : strftime fails on windows... (output empty)
			strftime(timebuf, 32, "%x %X %Z : ", timeinfo);
		}
		return std::string(timebuf);
	}

	///Output functions (put)
	///Write sequence of characters
	std::streamsize LogStreamBuf::xsputn(const char * s, std::streamsize n)
	{
		std::streamsize ressize = 0;
		if (ptm_filterin)
		{
			char* laststr = (char*)s;
			//last string
			ressize += std::stringbuf::xsputn(laststr, strlen(laststr));
		}
		else // filterout
		{
			//fake a proper sputn.
			ressize += n;
		}
		return ressize;//ressize == 0 means something is wrong -> will set ios::failbit
	}

	int LogStreamBuf::overflow(int c)
	{
		int res = 0;
		if (ptm_filterin)
		{
			res = std::stringbuf::overflow(c);
			//TOTHINK ABOUT : we can do it here instead of in sync... if we want it into the stringbuf...
			//            std::string timestr = getlocaltime();
			//    sputn(timestr.c_str(),timestr.length());
		}
		else
		{
			res = 0; //nothing to do, message filtered out
		}
		return res;
	}


	/***************** For clogstreambuff : to output to clog *******/


	CLogStreamBuf::CLogStreamBuf()
		: LogStreamBuf(), ptm_clogbuf(std::clog.rdbuf())//using clog as sink
	{
	}

	CLogStreamBuf::~CLogStreamBuf()
	{
	}

	///Synchronizes (flush) the stream buffer
	int CLogStreamBuf::sync()
	{
		int res = 0;

		//we add prefix only if we are in filterin state
		if (ptm_filterin)
		{

			if (ptm_logtime)
			{
				//TODO : speed up with fixed string size
				//passon prefixes strings
				std::string timestr = getlocaltime();
				ptm_clogbuf->sputn(timestr.c_str(), timestr.length());
			}

			ptm_clogbuf->sputn(ptm_prefix.c_str(), ptm_prefix.length());

			//transfer ptm_buf to clogbuf
			//copy character one by one...
			//maybe better to lock buffer, and get it whole at once...
			char c = LogStreamBuf::sbumpc();
			while (c != EOF)
			{
				ptm_clogbuf->sputc(c);
				c = LogStreamBuf::sbumpc();
			}

		}


		res += ptm_clogbuf->pubsync();

		return res;
	}


	/***************** For filelogstreambuf : to output to file *******/


	FileLogStreamBuf::FileLogStreamBuf(const std::string & filename)
		: LogStreamBuf(), ptm_filelogbuf()//using clog as sink
	{
		//TOTHINK ABOUT : do we delete the file everytime we open it ?
		//or provide an option bool append for example ?
		if (0 == ptm_filelogbuf.open(filename.c_str(), std::ios::out | std::ios::app))
			throw std::logic_error("unable to open" + filename);
	}

	FileLogStreamBuf::~FileLogStreamBuf()
	{
		ptm_filelogbuf.close();
	}

	///Synchronizes (flush) the stream buffer
	int FileLogStreamBuf::sync()
	{
		int res = 0;
		if (ptm_filelogbuf.is_open())
		{
			//we add prefix only if we are in filterin state
			if (ptm_filterin)
			{

				if (ptm_logtime)
				{
					//TODO : speed up with fixed string size
					//passon prefixes strings
					std::string timestr = getlocaltime();
					ptm_filelogbuf.sputn(timestr.c_str(), timestr.length());
				}


				//transfer ptm_buf to clogbuf
				//copy character one by one...
				//maybe better to lock buffer, and get it whole at once...
				char c = LogStreamBuf::sbumpc();
				while (c != EOF)
				{
					ptm_filelogbuf.sputc(c);
					c = LogStreamBuf::sbumpc();
				}

			}


			res += ptm_filelogbuf.pubsync();
		}
		return res;
	}




} //WkCocos