LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := wkcocosapp

LOCAL_MODULE_FILENAME := libwkcocosapp

LOCAL_SRC_FILES := main.cpp \
                   ../../../../../Classes/App/AppDelegate.cpp \
                   ../../../../../Classes/App/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_SHARED_LIBRARIES := wkcocos_shared

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
# $(call import-module,entityx)



$(call import-module,lib/src/main/jni)


