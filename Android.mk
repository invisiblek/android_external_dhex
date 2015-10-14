LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    buffers.c \
    configfile.c \
    correlation.c \
    gpl.c \
    hexcalc.c \
    input.c \
    machine_type.c \
    main.c \
    markers.c \
    menu.c \
    output.c \
    search.c \
    ui.c

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    external/libncurses/include

LOCAL_SHARED_LIBRARIES += \
    libncurses

LOCAL_CFLAGS += -DDHEXRC_PATH=\"/system/etc/dhexrc\"

LOCAL_MODULE := dhex
LOCAL_MODULE_TAGS := debug
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE       := dhexrc
LOCAL_MODULE_TAGS  := debug
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := dhexrc
include $(BUILD_PREBUILT)
