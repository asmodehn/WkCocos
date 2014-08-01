#include "WkCocos/Utils/ToolBox.h"

//for mkdir
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#endif

namespace WkCocos
{
	ToolBox::ToolBox()
	{}

	ToolBox::~ToolBox()
	{}
	
	std::string ToolBox::itoa(int value)
	{
		std::stringstream str;
		str << value;

		return str.str();
	}
	
	/* Opens the file, creating directories in its pathspec if necessary */
	FILE* ToolBox::FOpen(std::string name, std::string mode)
	{
		size_t slashpos = name.find("/", 0 );
		while (slashpos != std::string::npos)
		{
			//directory found
			std::string dirname = name.substr(0, slashpos);
			std::string dirfullname = dirname;
			if (!cocos2d::FileUtils::getInstance()->isFileExist(dirfullname))
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				mkdir(dirfullname.c_str());
#else
				mkdir(dirfullname.c_str(),0770);
#endif
			}

			//checking next dir
			slashpos = name.find("/", ++slashpos);
		}

		return fopen(name.c_str(), mode.c_str());
	}

	unsigned long ToolBox::stoul(const std::string& _Str, size_t *_Idx, int _Base)
	{	// convert string to unsigned long
		const char *_Ptr = _Str.c_str();
		char *_Eptr;
		errno = 0;
		unsigned long _Ans = strtoul(_Ptr, &_Eptr, _Base);

		if (_Ptr == _Eptr)
			throw std::invalid_argument("invalid stoul argument");
		if (errno == ERANGE)
			throw std::out_of_range("stoul argument out of range");
		if (_Idx != 0)
			*_Idx = (size_t)(_Eptr - _Ptr);
		return (_Ans);
	}


	///Get Current Time
	struct tm ToolBox::getLocalTime()
	{
		time_t rawtime;
		struct tm * timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		CCLOG("Current local time and date: %s", asctime(timeinfo));

		//Copy of POD tm
		return *timeinfo;
	}

	///Get Current Time
	struct tm ToolBox::getUTCTime()
	{
		time_t rawtime;
		struct tm * ptm;

		time(&rawtime);

		ptm = gmtime(&rawtime);

		CCLOG("Current UTC time and date: %s", asctime(ptm));

		//Copy of POD tm
		return *ptm;
	}

}//namespace dfgame
