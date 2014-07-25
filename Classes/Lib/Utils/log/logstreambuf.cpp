#include "WkCocos/Utils/log/logstreambuf.h"

#include <stdexcept>
#include <cstring>

namespace WkCocos
{

	LogStreamBuf::LogStreamBuf()
		: std::stringbuf() //stringbuf in and out required
		, ptm_prefix("") //default no prefix
		, ptm_logtime(true)
		, ptm_filterin(true)
		, m_onSync(nullptr)
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

	int LogStreamBuf::sync()
	{
		if (m_onSync)
		{
			m_onSync();
		}
		str("");
		return 0;
	}


} //WkCocos