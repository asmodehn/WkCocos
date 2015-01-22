#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "WkCocos/Utils/GPGSManager.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

/**
#include "WkCocosApp/AppDelegate.h"

using namespace cocos2d;

void cocos_android_app_init (JNIEnv* env, jobject thiz) {
    LOGD("cocos_android_app_init");
    AppDelegate *pAppDelegate = new AppDelegate();
}
**/


extern "C"
{
JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeInitGPGS(JNIEnv*  env, jobject thiz, jobject activity)
{
    gpg::AndroidPlatformConfiguration platform_configuration;
    platform_configuration.SetActivity(activity);
    GPGSManager::getInstance()->InitServices(platform_configuration);

}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityResult(
    JNIEnv* env, jobject thiz, jobject activity, jint requestCode,
    jint resultCode, jobject data) {
  gpg::AndroidSupport::OnActivityResult(env, activity, requestCode, resultCode, data);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityCreated(
    JNIEnv* env, jobject thiz, jobject activity, jobject saved_instance_state) {
  gpg::AndroidSupport::OnActivityCreated(env, activity, saved_instance_state);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityDestroyed(
    JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityDestroyed(env, activity);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityPaused(
    JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityPaused(env, activity);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityResumed(
    JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityResumed(env, activity);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivitySaveInstanceState(
    JNIEnv* env, jobject thiz, jobject activity, jobject out_state) {
  gpg::AndroidSupport::OnActivitySaveInstanceState(env, activity, out_state);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityStarted(
    JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityStarted(env, activity);
}

JNIEXPORT void
Java_com_asmodehn_wkcocos_lib_MainActivity_nativeOnActivityStopped(
    JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityStopped(env, activity);
}

}