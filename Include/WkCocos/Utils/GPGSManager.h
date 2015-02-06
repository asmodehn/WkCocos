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
    , currentSnapshot("")
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
	void incrementAchievement(const std::string & achievement_id, int step);
	void showAchievements();

    void saveSnapshot(std::string description, std::chrono::milliseconds playtime, std::vector<uint8_t> png_data, std::vector< uint8_t > snapData);
    void selectSnapshot(std::string title, uint32_t max_snapshots, bool allow_delete = true, bool allow_create = true);

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

	struct SnapshotSaveRequested : public entityx::Event < SnapshotSaveRequested >
	{
		SnapshotSaveRequested()
		{}
	};

	struct SnapshotLoadRequested : public entityx::Event < SnapshotLoadRequested >
	{
		SnapshotLoadRequested()
		{}
	};

    struct SnapshotLoaded : public entityx::Event < SnapshotLoaded >
    {
        bool mSuccess;
        std::vector< uint8_t > mSnapData;

        SnapshotLoaded(bool success, std::vector<uint8_t> data)
        : mSuccess(success)
        , mSnapData(data)
        {}

    };

    struct SnapshotSaved : public entityx::Event < SnapshotSaved >
    {
        bool mSuccess;
        std::string mDescription;
        std::chrono::milliseconds mPlayedTime;
        std::chrono::milliseconds mModifiedTime;

        SnapshotSaved(bool success, std::string desc, std::chrono::milliseconds playtime, std::chrono::milliseconds modtime)
        : mSuccess(success)
        , mDescription(desc)
        , mPlayedTime(playtime)
        , mModifiedTime(modtime)
        {}

    };

    entityx::ptr<entityx::EventManager> getEventManager()
    {
        return event_manager;
    }
private:

    bool isSignedIn;
    std::string currentSnapshot;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void OnAuthActionStarted(gpg::AuthOperation op);
    void OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status);

    void loadedSnapshot(gpg::SnapshotManager::ReadResponse const & response);
    void committedSnapshot(gpg::SnapshotManager::CommitResponse const & response);
    void selectedSnapshot(gpg::SnapshotManager::SnapshotSelectUIResponse const & response);

    std::unique_ptr<gpg::GameServices> gameServices;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else
#endif
    entityx::ptr<entityx::EventManager> event_manager;
    static GPGSManager* instance;
};





#endif // GPGS_MANAGER_HPP
