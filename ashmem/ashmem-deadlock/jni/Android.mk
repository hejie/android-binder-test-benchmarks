LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	exp.c

LOCAL_SHARED_LIBRARIES := \
  libutils \
  libcutils \
  libashmem \
  libbinder

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= exp

include $(BUILD_EXECUTABLE)
