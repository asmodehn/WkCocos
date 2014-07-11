LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := wkcocos_static

LOCAL_MODULE_FILENAME := libwkcocos

LOCAL_SRC_FILES := jni.cpp \
                   ../../../../../Classes/Lib/Utils/ToolBox.cpp \
                   ../../../../../Classes/Lib/Options.cpp \
                   ../../../../../Classes/Lib/Player.cpp \
                   ../../../../../Classes/Lib/Save/Local.cpp \
                   ../../../../../Classes/Lib/Save/Online.cpp \
                   ../../../../../Classes/Lib/App42/Setup.cpp \
                   ../../../../../Classes/Lib/App42/Login.cpp\
                   ../../../../../Classes/Lib/Utils/CocosAppender.cpp\
                   ../../../../../Classes/Lib/Utils/Logger.cpp\
                   ../../../../../Classes/Lib/Utils/log/logappender.cpp\
                   ../../../../../Classes/Lib/Utils/log/loglevel.cpp\
                   ../../../../../Classes/Lib/Utils/log/logstream.cpp\
                   ../../../../../Classes/Lib/Utils/log/logstreambuf.cpp\
                   ../../../../../Classes/Lib/Utils/log/nullstream.cpp\
                   ../../../../../Classes/Lib/Utils/log/nullstreambuf.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
#LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += entityx_static
LOCAL_WHOLE_STATIC_LIBRARIES += app42SDK_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
#$(call import-module,Box2D)
#$(call import-module,editor-support/cocosbuilder)
#$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
$(call import-module,App42_Cocos2DX_SDK)


