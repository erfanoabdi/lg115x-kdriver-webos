#-----------------------------------------------------------------------------------------
# arch_dev.mk describles the default device description
#-----------------------------------------------------------------------------------------
include $(KDRV_TOP_DIR)/../chip/m14/os/arch_dev.mk

INCLUDE_KDRV_LOGM       := YES

ccflags-y += -D__ANDROID__
