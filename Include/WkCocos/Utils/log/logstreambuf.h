#ifndef __WKCOCOS_UTILS_LOG_LOGSTREAMBUF_H__
#define __WKCOCOS_UTILS_LOG_LOGSTREAMBUF_H__

#include <cstdio> // for EOF
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <functional>

namespace WkCocos
{
	/**
	* \class logstreambuf
	*
	* \brief This class defines an overloaded streambuf, to be used with logstream
	* Retrieve the log message and callback LogStream when syncing, to output to appenders
	*
	* Date&Time (internally added) [TODO| ThreadID (internally added)] |Custom Prefix (set by logstream) | Message ( sent by logstream )
	*/

	class LogStreamBuf : public std::stringbuf
	{
	public:
		/**
		* Default constructor
		*/
		LogStreamBuf();

		/**
		* Destructor
		*/
		virtual ~LogStreamBuf();

		//to manage prefix
		void resetprefix(const std::string& newprefix = 0);
		const std::string& getprefix() const;

		void resetlogtime(bool v = true)
		{
			ptm_logtime = v;
		}

		/**
		* Register syncing callback
		*/
		inline void registerOnSync(std::function<void()> syncCallback){ m_onSync = syncCallback; }

		/**
		* to use logstream as streamthrough
		*/
		friend std::ostream& operator<<(std::ostream& o, LogStreamBuf & l)
		{
			return o << l.str();
		};
		friend std::ostream& operator<<(std::ostream& o, const LogStreamBuf & l)
		{
			return o << l.str();
		};

		// REMOVE?
		// Buffer should always keep message
		// filtering will be done by appenders
		//to start and stop getting messages
		void filterout()
		{
			ptm_filterin = false;
		}
		void filterin()
		{
			ptm_filterin = true;
		}

	protected:
		/**
		* When sync, call onSync Callback and reset the buffer
		* Based on design, reset shouldnt happen if you want to keep a full log, and be able to sort it again on your different appender.
		*/
		virtual int sync();

		/**
		* Get the current time
		*/
		std::string getlocaltime();

	protected:
		// REMOVE
		std::string ptm_prefix;
		bool ptm_logtime;
		//filter boolean
		bool ptm_filterin;

		/**
		* Callback to sync the appender
		*/
		std::function<void()> m_onSync;
	};

}// WkCocos

#endif // __WKCOCOS_UTILS_LOG_LOGSTREAMBUF_H__