#include "main.h"
#include "WkCocosApp/AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int main(int argc, char* argv[])
{
	// create the application instance
	srand((unsigned int)time(NULL));
	AppDelegate app;
	return Application::getInstance()->run();
}