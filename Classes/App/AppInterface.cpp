#include "WkCocosApp/AppInterface.h"


AppInterface::AppInterface()
{}

AppInterface::AppInterface(std::string filepath)
: WkCocos::Interface(filepath)
{}

AppInterface::~AppInterface()
{}

void AppInterface::onShow()
{}

void AppInterface::onHide()
{}
