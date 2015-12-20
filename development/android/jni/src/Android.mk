LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_image.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_image
LOCAL_STATIC_LIBRARIES := SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_mixer.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_mixer
LOCAL_STATIC_LIBRARIES := SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/boost

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../../*.cpp)

LOCAL_STATIC_LIBRARIES := SDL2_static SDL2_image_static SDL2_mixer_static

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lz -landroid

include $(BUILD_SHARED_LIBRARY)
