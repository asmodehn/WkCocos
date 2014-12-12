#ifndef GPGS_MANAGER_H
#define GPGS_MANAGER_H

#include "cocos/cocos2d.h"

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "gpg/gpg.h"

class GPGSManager {

public:
    static void InitServices(gpg::PlatformConfiguration &pc);
    static gpg::GameServices *GetGameServices();
    static void BeginUserInitiatedSignIn();
    static void SignOut();
    static bool IsSignedIn() { return isSignedIn; }

private:
    static bool isSignedIn;
    static std::unique_ptr<gpg::GameServices> gameServices;

};

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#endif



#endif // GPGS_MANAGER_HPP
