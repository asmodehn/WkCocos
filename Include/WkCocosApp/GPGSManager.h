#ifndef GPGS_MANAGER_H
#define GPGS_MANAGER_H

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

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


#endif // GPGS_MANAGER_HPP
