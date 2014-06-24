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

}//namespace dfgame
