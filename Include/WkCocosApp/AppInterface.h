#ifndef __APPINTERFACE_H__
#define __APPINTERFACE_H__

#include "WkCocos/Interface.h"

class AppInterface : public WkCocos::Interface
{
public:
	/**
	* Default constructor
	*/
	AppInterface();

	/**
	* constructor
	*/
	AppInterface(std::string filepath);

	/**
	* Destructor
	*/
	virtual ~AppInterface();

	/**
	* Event show
	*/
	virtual void onShow();

	/**
	* Event hide
	*/
	virtual void onHide();
};

#endif //__APPINTERFACE_H__