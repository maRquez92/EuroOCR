LOCAL_PATH := $(call my-dir)
 
#include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)

#LOCAL_MODULE    := ndkfoo
#LOCAL_SRC_FILES := ndkfoo.cpp

#include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := nonfree_prebuilt
LOCAL_SRC_FILES := libnonfree.so
LOCAL_LDLIBS     += -llog -ldl

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := opencv_java_prebuilt
LOCAL_SRC_FILES := libopencv_java.so
LOCAL_LDLIBS     += -llog -ldl

include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE    := leptonica_prebuilt
LOCAL_SRC_FILES := liblept.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := tesseract_prebuilt
LOCAL_SRC_FILES := libtess.so
include $(PREBUILT_SHARED_LIBRARY)
 
#include tesseract/tesseract.mk
 
 
include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)
LOCAL_C_INCLUDES := C:/Users/Paulo/Downloads/OpenCV-2.4.8-android-sdk/OpenCV-2.4.8-android-sdk/sdk/native/jni/include
#LOCAL_C_INCLUDES += $(TESSERACT_THIS_DIR)/include
LOCAL_C_INCLUDES += C:\toolkits\includes

LOCAL_MODULE    := ndkCV
LOCAL_CFLAGS    := -Werror -O3 -ffast-math
LOCAL_CPPFLAGS = -std=c++11

LOCAL_LDLIBS    += $(MY_LDLIBS) -llog -ldl 
LOCAL_SHARED_LIBRARIES := opencv_java_prebuilt nonfree_prebuilt tesseract_prebuilt leptonica_prebuilt # tess lept

LOCAL_SRC_FILES := ndkCV.cpp thresholder.cpp

include $(BUILD_SHARED_LIBRARY)