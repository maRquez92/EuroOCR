LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)

LOCAL_MODULE    := ndkfoo
LOCAL_SRC_FILES := ndkfoo.cpp

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := nonfree_prebuilt
LOCAL_SRC_FILES := libnonfree.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := opencv_java_prebuilt
LOCAL_SRC_FILES := libopencv_java.so
include $(PREBUILT_SHARED_LIBRARY)
 
 
include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)
LOCAL_C_INCLUDES:= C:/Users/Paulo/Downloads/OpenCV-2.4.8-android-sdk/OpenCV-2.4.8-android-sdk/sdk/native/jni/include
LOCAL_SHARED_LIBRARIES := opencv_java_prebuilt

LOCAL_MODULE    := thresholder

LOCAL_SRC_FILES := thresholder.cpp

include $(BUILD_SHARED_LIBRARY)
 
 
include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)
LOCAL_C_INCLUDES:= C:/Users/Paulo/Downloads/OpenCV-2.4.8-android-sdk/OpenCV-2.4.8-android-sdk/sdk/native/jni/include

LOCAL_MODULE    := ndkCV
LOCAL_SHARED_LIBRARIES := nonfree_prebuilt opencv_java_prebuilt thresholder

LOCAL_SRC_FILES := ndkCV.cpp

include $(BUILD_SHARED_LIBRARY)