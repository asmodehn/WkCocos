#ifndef GPGS_MANAGER_H
#define GPGS_MANAGER_H

#include "entityx/entityx.h"
#include "cocos/cocos2d.h"

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "gpg/gpg.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//TODO

#else

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#endif

//singleton design for simple access from jni implementation
class GPGSManager
{
    GPGSManager()
    : event_manager(entityx::EventManager::make())
    , isSignedIn(false)
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    , gameServices()
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//TODO
#else
#endif
    {}

    virtual ~GPGSManager()
    {
    }

public:

    static GPGSManager* getInstance()
    {
        if ( ! instance )
        {
            instance = new GPGSManager();
        }
        return instance;
    }


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void InitServices(gpg::PlatformConfiguration &pc);
    gpg::GameServices *GetGameServices();
    void BeginUserInitiatedSignIn();
    void SignOut();
    bool IsSignedIn();

    void unlockAchievement(const std::string & achievement_id);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else
#endif

    struct SignedIn : public entityx::Event < SignedIn >
    {

    };

    struct SignedOut : public entityx::Event < SignedOut >
    {

    };

    entityx::ptr<entityx::EventManager> getEventManager()
    {
        return event_manager;
    }
private:

    bool isSignedIn;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void OnAuthActionStarted(gpg::AuthOperation op);
    void OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status);

    std::unique_ptr<gpg::GameServices> gameServices;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else
#endif
    entityx::ptr<entityx::EventManager> event_manager;
    static GPGSManager* instance;
};





#endif // GPGS_MANAGER_HPP
