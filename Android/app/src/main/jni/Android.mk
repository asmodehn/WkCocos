LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := wkcocosapp

LOCAL_MODULE_FILENAME := libwkcocosapp

LOCAL_SRC_FILES := main.cpp \
                   ../../../../../Classes/App/AppDelegate.cpp \
                   ../../../../../Classes/App/HelloWorldScene.cpp \
                   ../../../../../Classes/App/LoadingScene.cpp \
                   ../../../../../Classes/App/LoadingUI.cpp \
                   ../../../../../Classes/App/ErrorUI.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_WHOLE_STATIC_LIBRARIES := wkcocos_static
# NOT WORKING because of cocos structure between C++ and Java
# LOCAL_SHARED_LIBRARIES := wkcocos_shared

include $(BUILD_SHARED_LIBRARY)

$(call import-module,lib/src/main/jni)





