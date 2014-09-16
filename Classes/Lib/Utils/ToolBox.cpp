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

}//namespace dfgame
