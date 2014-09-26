# use this to select gcc instead of default 4.6
NDK_TOOLCHAIN_VERSION := 4.8
# OR use this to select the latest clang version:
#NDK_TOOLCHAIN_VERSION := clang

#This is needed to get GLES2 working
APP_PLATFORM := android-9

#building for all platforms but our minimal supported ABI is ARMv7A
APP_ABI := armeabi armeabi-v7a x86

APP_STL := gnustl_static
APP_CPPFLAGS :=  -std=c++11 -fsigned-char -frtti -fexceptions

#cocos flag
APP_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1
