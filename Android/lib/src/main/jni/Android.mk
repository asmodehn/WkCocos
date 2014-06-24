LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := wkcocos_static

LOCAL_MODULE_FILENAME := libwkcocos

LOCAL_SRC_FILES := jni.cpp \
                   ../../../../../Classes/Lib/Utils/ToolBox.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
# LOCAL_WHOLE_STATIC_LIBRARIES += entityx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
# $(call import-module,entityx)


