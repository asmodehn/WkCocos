#include "WkCocos/Utils/log/logstream.h"

namespace WkCocos
{

	LogStream::LogStream()
		: std::ostream(&m_buffer)
	{
		//setup default flags
		this->flags(std::ios::left | std::ios::dec | std::ios::showbase | std::ios::boolalpha);

		m_buffer.registerOnSync(std::bind(&LogStream::syncAppenders, this));

	}

	LogStream::~LogStream()
	{
	}
	
	LogStream& operator<<(LogStream &ls, loglevel::Level lvl)
	{
		if (ls.getLevel() >= lvl)
		{
			//ls.rdbuf()->filterin();
			//dynamic casting to call the ostream << ( ostream, loglevel) operator
			dynamic_cast<std::ostream&>(ls) << lvl;
		}
		else
		{
			//ls.rdbuf()->filterout();
		}
		return ls;
	}

	LogStream& LogStream::level(loglevel::Level l)
	{
		*this << l;
		return *this;
	}

	void LogStream::syncAppenders()
	{
		for (auto appender : _appenders)
		{
			(*appender) << m_buffer;
		}
	}

	void LogStream::removeAppender(LogAppender* oldAppender)
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
} // WkCocos