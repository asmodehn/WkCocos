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

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

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
        LOGI("Signing In.");
        CCLOGERROR("EMITTING");
        event_manager->emit<GPGSManager::SignedIn>();
    }
    else{
        LOGI("Signing Out.");
        CCLOGERROR("EMITTING");
        event_manager->emit<GPGSManager::SignedOut>();
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
      }).Create(pc);
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

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

//TODO
#else
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
