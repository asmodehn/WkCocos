#include "WkCocos/Utils/GPGSManager.h"

#ifdef __APPLE__
//
//Logging for CoreFoundation
//
#include <CoreFoundation/CoreFoundation.h>
extern "C" void NSLog(CFStringRef format, ...);
const int32_t BUFFER_SIZE = 256;
#define LOGI(...) {char c[BUFFER_SIZE];\
    snprintf(c,BUFFER_SIZE,__VA_ARGS__);\
    CFStringRef str = CFStringCreateWithCString(kCFAllocatorDefault, c, kCFStringEncodingMacRoman);\
    NSLog(str);\
    CFRelease(str);\
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#define DEBUG_TAG "main"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))

#endif

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

GPGSManager* GPGSManager::instance = nullptr;

//cocos style platform detection
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GPGSManager::OnAuthActionStarted(gpg::AuthOperation op) {
  LOGI("OnAuthActionStarted");
  switch ( op ) {
  case gpg::AuthOperation::SIGN_IN:
    LOGI("Signing In...");
    break;
  case gpg::AuthOperation::SIGN_OUT:
    LOGI("Signing Out...");
    break;
  }
}

void GPGSManager::OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status) {
    LOGI("OnAuthActionFinished");
    if (op == gpg::AuthOperation::SIGN_IN){
        LOGI("Signed In.");
        cocos2d::Director* d = cocos2d::Director::getInstance();
        if ( d )
        {
            cocos2d::Scheduler* s = d->getScheduler();
            if (s)
            {
                s->performFunctionInCocosThread([=](){
                    CCLOGERROR("EMITTING");
                    event_manager->emit<GPGSManager::SignedIn>();
                });
            }
        }
    }
    else{
        LOGI("Signed Out.");
        cocos2d::Director* d = cocos2d::Director::getInstance();
        if ( d )
        {
            cocos2d::Scheduler* s = d->getScheduler();
            if (s)
            {
                s->performFunctionInCocosThread([=](){
                    CCLOGERROR("EMITTING");
                    event_manager->emit<GPGSManager::SignedOut>();
                });
            }
        }
    }
}

gpg::GameServices *GPGSManager::GetGameServices() {
  return gameServices.get();
}

bool GPGSManager::IsSignedIn()
{
    return isSignedIn;
}

void GPGSManager::BeginUserInitiatedSignIn() {
  if (gameServices && !gameServices->IsAuthorized()) {
    LOGI("StartAuthorizationUI");
    gameServices->StartAuthorizationUI();
  }
}

void GPGSManager::SignOut() {
  if (gameServices && gameServices->IsAuthorized()) {
    LOGI("SignOut");
    gameServices->SignOut();
  }
}

void GPGSManager::InitServices(gpg::PlatformConfiguration &pc)
{
  LOGI("Initializing Services");
  if (!gameServices) {
    LOGI("Uninitialized services, so creating");
    gameServices = gpg::GameServices::Builder()
      .SetLogging(gpg::DEFAULT_ON_LOG, gpg::LogLevel::VERBOSE)
      // Add a test scope (we don't actually use this).
      //    .AddOauthScope("https://www.googleapis.com/auth/appstate")
      //    .InternalSetRootURL("https://www-googleapis-staging.sandbox.google.com/")
      .SetOnAuthActionStarted([this](gpg::AuthOperation op){
          this->OnAuthActionStarted(op);
      })
      .SetOnAuthActionFinished([this](gpg::AuthOperation op, gpg::AuthStatus status){
          LOGI("Sign in finished with a result of %d", status);
          if( status == gpg::AuthStatus::VALID )
              isSignedIn = true;
          else
              isSignedIn = false;
          this->OnAuthActionFinished( op, status);
      })
      .EnableSnapshots()
      .Create(pc);
  }
  if (gameServices)
  {
    LOGI("Created");
  }
  else
  {
    LOGE("GameServices creation FAILED !");
  }
}

void GPGSManager::unlockAchievement(const std::string & achievement_id)
{
	if ( gameServices )
	{
		gameServices->Achievements().Unlock(achievement_id);
	}
}

void GPGSManager::incrementAchievement(const std::string & achievement_id, int step)
{
	if (gameServices)
	{
		gameServices->Achievements().Increment(achievement_id, step);
	}
}

void GPGSManager::showAchievements()
{
	if (gameServices)
	{
		if(!isSignedIn)
		{
			BeginUserInitiatedSignIn();
		}
		else
		{
			gameServices->Achievements().ShowAllUIBlocking();
		}
	}
}

void GPGSManager::committedSnapshot(gpg::SnapshotManager::CommitResponse const & response)
{
    bool success = false;
    if (response.status == gpg::ResponseStatus::VALID)
    {
        LOGI("Snapshot CommitResponse VALID");
        bool success = true;
    }
    else if (response.status == gpg::ResponseStatus::VALID_BUT_STALE)
    {
        LOGI("Snapshot CommitResponse VALID_BUT_STALE");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_LICENSE_CHECK_FAILED)
    {
        LOGI("Snapshot CommitResponse ERROR_LICENSE_CHECK_FAILED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_INTERNAL)
    {
        LOGI("Snapshot CommitResponse ERROR_INTERNAL");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_NOT_AUTHORIZED)
    {
        LOGI("Snapshot CommitResponse ERROR_NOT_AUTHORIZED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_VERSION_UPDATE_REQUIRED)
    {
        LOGI("Snapshot CommitResponse ERROR_VERSION_UPDATE_REQUIRED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_TIMEOUT)
    {
        LOGI("Snapshot CommitResponse ERROR_TIMEOUT");
    }

    if (response.data.Valid())
    {
        cocos2d::Director* d = cocos2d::Director::getInstance();
        if ( d )
        {
            cocos2d::Scheduler* s = d->getScheduler();
            if (s)
            {
                s->performFunctionInCocosThread([=](){
                    CCLOGERROR("EMITTING GPGSManager::SnapshotSaved");
                    event_manager->emit<GPGSManager::SnapshotSaved>(true, response.data.Description(), response.data.PlayedTime(), response.data.LastModifiedTime());
                });
            }
        }
    }
}

void GPGSManager::loadedSnapshot(gpg::SnapshotManager::ReadResponse const & response)
{
    if (response.status == gpg::ResponseStatus::VALID)
    {
        LOGI("Snapshot ReadResponse VALID");
        cocos2d::Director* d = cocos2d::Director::getInstance();
        if ( d )
        {
            cocos2d::Scheduler* s = d->getScheduler();
            if (s)
            {
                s->performFunctionInCocosThread([=](){
                    CCLOGERROR("EMITTING GPGSManager::SnapshotLoaded");
                    event_manager->emit<GPGSManager::SnapshotLoaded>(true, response.data);
                });
            }
        }
    }
    else if (response.status == gpg::ResponseStatus::VALID_BUT_STALE)
    {
        LOGI("Snapshot ReadResponse VALID_BUT_STALE");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_LICENSE_CHECK_FAILED)
    {
        LOGI("Snapshot ReadResponse ERROR_LICENSE_CHECK_FAILED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_INTERNAL)
    {
        LOGI("Snapshot ReadResponse ERROR_INTERNAL");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_NOT_AUTHORIZED)
    {
        LOGI("Snapshot ReadResponse ERROR_NOT_AUTHORIZED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_VERSION_UPDATE_REQUIRED)
    {
        LOGI("Snapshot ReadResponse ERROR_VERSION_UPDATE_REQUIRED");
    }
    else if (response.status == gpg::ResponseStatus::ERROR_TIMEOUT)
    {
        LOGI("Snapshot ReadResponse ERROR_TIMEOUT");
    }
}


void GPGSManager::selectedSnapshot(gpg::SnapshotManager::SnapshotSelectUIResponse const & response)
{
    if (response.status == gpg::UIStatus::VALID)
    {
        LOGI("Snapshot SelectUIResponse VALID");

        if ( response.data.Valid() )
        {
            currentSnapshot = response.data.FileName();
            if (gameServices)
            {
                LOGI("Loading Snapshot %s",currentSnapshot.c_str());
                gameServices->Snapshots().Open(response.data.FileName(),
                    gpg::SnapshotConflictPolicy::LONGEST_PLAYTIME,
                    [this](gpg::SnapshotManager::OpenResponse const & response)
                    {
                        LOGI("Reading file");
                        gameServices->Snapshots().Read(response.data, std::bind(&GPGSManager::loadedSnapshot, this, std::placeholders::_1));
                    });
            }
        }
        else // no selected snapshot to load -> we should save a new one
        {
            currentSnapshot.clear();
            cocos2d::Director* d = cocos2d::Director::getInstance();
            if ( d )
            {
                cocos2d::Scheduler* s = d->getScheduler();
                if (s)
                {
                    s->performFunctionInCocosThread([=](){
                        CCLOGERROR("EMITTING GPGSManager::SnapshotSaveRequested");
                        event_manager->emit<GPGSManager::SnapshotSaveRequested>();
                    });
                }
            }
        }
    }
    else if (response.status == gpg::UIStatus::ERROR_INTERNAL)
    {
        LOGI("Snapshot SelectUIResponse ERROR_INTERNAL");
    }
    else if (response.status == gpg::UIStatus::ERROR_NOT_AUTHORIZED)
    {
        LOGI("Snapshot SelectUIResponse ERROR_NOT_AUTHORIZED");
    }
    else if (response.status == gpg::UIStatus::ERROR_VERSION_UPDATE_REQUIRED)
    {
        LOGI("Snapshot SelectUIResponse ERROR_VERSION_UPDATE_REQUIRED");
    }
    else if (response.status == gpg::UIStatus::ERROR_TIMEOUT)
    {
        LOGI("Snapshot SelectUIResponse ERROR_TIMEOUT");
    }
    else if (response.status == gpg::UIStatus::ERROR_CANCELED)
    {
        LOGI("Snapshot SelectUIResponse ERROR_CANCELED");
    }
    else if (response.status == gpg::UIStatus::ERROR_UI_BUSY)
    {
        LOGI("Snapshot SelectUIResponse ERROR_UI_BUSY");
    }
    else if (response.status == gpg::UIStatus::ERROR_LEFT_ROOM)
    {
        LOGI("Snapshot SelectUIResponse ERROR_LEFT_ROOM");
    }
}

void GPGSManager::saveSnapshot(std::string description, std::chrono::milliseconds playtime, std::vector<uint8_t> png_data, std::vector< uint8_t > snapData)
{
    if (gameServices)
	{
	    //if we do not have current snapshot we generate the filename
	    if (currentSnapshot.empty())
        {

            std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
            std::time_t tt = std::chrono::system_clock::to_time_t ( today );

            //using strftime
            time_t rawtime;
            struct tm * timeinfo;
            char buffer [32];

            time (&tt);
            timeinfo = localtime (&rawtime);

            strftime (buffer,32,"save_%Y%m%d%H%M%S",timeinfo);

            //put_time not available in GCC < 5.0
            //std::ostringstream snapshot_name;
            //snapshot_name << std::put_time(ctime(&tt),"save_%Y%m%d%H%M%S");
            currentSnapshot.assign(buffer,32);
            LOGI("new snapshot name is: %s",currentSnapshot.c_str());
        }

	    LOGI("Saving Snapshot %s",currentSnapshot.c_str());
	    LOGI("%s",description.c_str());
        gameServices->Snapshots().Open(currentSnapshot,
            gpg::SnapshotConflictPolicy::LONGEST_PLAYTIME,
            [this, description, playtime, png_data, snapData](gpg::SnapshotManager::OpenResponse const &response)
            {
                gpg::SnapshotMetadata metadata;
                if (IsSuccess(response.status)) {
                    metadata = response.data;

                    gpg::SnapshotMetadataChange::Builder builder;
                    if ( description != metadata.Description())
                    {
                        builder.SetDescription(description);
                    }
                    if ( playtime != metadata.PlayedTime())
                    {
                        builder.SetPlayedTime(static_cast<gpg::Duration>(playtime));
                    }
                    if ( ! png_data.empty() )
                    {
                        builder.SetCoverImageFromPngData(png_data);
                    }
                    gpg::SnapshotMetadataChange metadata_change = builder.Create();
                    gameServices->Snapshots().Commit(metadata, metadata_change,snapData,std::bind(&GPGSManager::committedSnapshot,this,std::placeholders::_1));

                } else {
                    // Handle snapshot open error here
                }
            }
        );

	}
}

void GPGSManager::selectSnapshot(std::string title, uint32_t max_snapshots, bool allow_create, bool allow_delete)
{
    if (gameServices)
	{
	    LOGI("Listing Snapshot");
	    LOGI("%s",title.c_str());

        gameServices->Snapshots().ShowSelectUIOperation( allow_create, allow_delete, max_snapshots, title, std::bind(&GPGSManager::selectedSnapshot, this, std::placeholders::_1) );
    }
}


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
