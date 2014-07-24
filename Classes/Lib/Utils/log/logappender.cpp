#include "WkCocos/Utils/log/logappender.h"
#include "WkCocos/Utils/log/loglevel.h"

#include <stdexcept>

namespace WkCocos
{

	LogAppender::LogAppender()
		: _level(WkCocos::loglevel::Core_LogDebug)
	{
		//_stream.getBuf()->onOverflow(std::bind(&LogAppender::onOverflow, this, std::placeholders::_1));
	}

	LogAppender::~LogAppender()
	{}

	void LogAppender::onOverflow(int c)
	{
		*this << _stream;
		std::clog << static_cast<char>(c);
	}
	
	CLogAppender::CLogAppender()
	{
	}

	CLogAppender::~CLogAppender()
	{}

	LogAppender& CLogAppender::operator << (const LogStream & msg)
	{
		std::clog << _stream;
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
	
	LogAppender& FileLogAppender::operator << (const LogStream & msg)
	{
		if (_ofstr.is_open())
		{
			if (_stream.tellp() > 1000)
			{
				_ofstr << _stream;
			}
		}
		return *this;
	}
	

}//namespace WkCocos