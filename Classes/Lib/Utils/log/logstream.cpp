#include "WkCocos/Utils/log/logstream.h"

namespace WkCocos
{

	LogStream::LogStream()
		: std::ostream(&pvm_lsb)
	{
			//to hook up to usual stream design
			/*pvm_lsb = new CLogStreamBuf();
			this->init(pvm_lsb);
*/
			//setup default flags
			this->flags(std::ios::left | std::ios::dec | std::ios::showbase | std::ios::boolalpha);

			//setup default prefix
			resetprefix();

			pvm_lsb.registerOnSync(std::bind(&LogStream::syncAppenders, this));

	}

	LogStream::LogStream(LogStreamBuf* lsb)
		: std::ostream(&pvm_lsb)
	{
			//to hook up to usual stream design
			//pvm_lsb = lsb;
			//this->init(pvm_lsb);

			//setup default flags
			this->flags(std::ios::left | std::ios::dec | std::ios::showbase | std::ios::boolalpha);

			//setup default prefix
			resetprefix();

		}

	LogStream::~LogStream()
	{
	}

	//to manage prefix
	void LogStream::resetprefix(const std::string& newprefix)
	{
		rdbuf()->resetprefix(newprefix);
	}

	const std::string & LogStream::getprefix() const
	{
		return rdbuf()->getprefix();
	}


	LogStream& operator<<(LogStream &ls, loglevel::Level lvl)
	{
		if (ls.getLevel() >= lvl)
		{
			ls.rdbuf()->filterin();
			//dynamic casting to call the ostream << ( ostream, loglevel) operator
			dynamic_cast<std::ostream&>(ls) << lvl;
		}
		else
		{
			ls.rdbuf()->filterout();
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
			//(*appender) << *this;
			(*appender) << pvm_lsb;
		}
	}

} // WkCocos