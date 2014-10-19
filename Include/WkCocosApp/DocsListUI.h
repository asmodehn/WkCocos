#ifndef __DOCSLISTUI_H__
#define __DOCSLISTUI_H__

//#include "cocos/ui/CocosGUI.h"

#include "WkCocos/Interface.h"
//#include "WkCocos/OnlineData/OnlineDataManager.h"

/**
* This UI is the main menu UI
*/
class DocsListUI : public WkCocos::Interface
{
public:
		
	static const std::string id;

	/**
	* Default constructor
	*/
	DocsListUI();

	/**
	* Destructor
	*/
	virtual ~DocsListUI();

protected:

};

#endif // __DOCSLISTUI_H__