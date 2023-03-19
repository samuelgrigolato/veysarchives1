LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../common/src/main.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/core/navigation.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/core/positioning.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/core/resources.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/core/audio.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/world/components/options-button.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/world/components/map.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/world/components/main-character.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/world/components/mobile-motion-controller.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/world/world.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../common/src/screens/home.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/core/log.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/jni_onload.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../common/include

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image png16

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,prefab/SDL2)
$(call import-module,prefab/SDL2_image)
$(call import-module,prefab/png16)
