#include "WkCocos/Utils/log/logappender.h"
#include "WkCocos/Utils/log/loglevel.h"

#include <stdexcept>

namespace WkCocos
{

	LogAppender::LogAppender()
		: _level(WkCocos::loglevel::Core_LogDebug)
	{
	}

	LogAppender::~LogAppender()
	{}
		
	CLogAppender::CLogAppender()
	{
	}

	CLogAppender::~CLogAppender()
	{}

	LogAppender& CLogAppender::operator << (const LogStreamBuf & msg)
	{
		std::clog << msg;
		return *this;
	}

	FileLogAppender::FileLogAppender(const std::string& fileName)
	{
		_ofstr.open(fileName.c_str(), std::ios::out | std::ios::app);
		if (!_ofstr.is_open())
		{
			throw std::logic_error("unable to open" + fileName);
		}
	}
	
	FileLogAppender::~FileLogAppender()
	{
		_ofstr.close();
	}
	
	LogAppender& FileLogAppender::operator << (const LogStreamBuf & msg)
	{
		if (_ofstr.is_open())
		{
			_ofstr << msg;
		}
		return *this;
	}
	

}//namespace WkCocos