
dxipc_src_top := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))


obj-y += dxipc.o
dxipc-objs += dxipc_drv.o
dxipc-objs += sc_if.o
ifeq ($(CONFIG_XTENSA_PLATFORM_D14VDEC),y)
dxipc-objs += suk.o
endif

FIRMWARE_VERSION := $(shell cd $(dxipc_src_top); if test -d .git; then git log --oneline -1 | sed 's/ .*$$//'; else echo 9999; fi)
ccflags-y += -DFIRMWARE_VERSION=\"$(FIRMWARE_VERSION)\"
ccflags-y += -I$(dxipc_src_top)/../hma
ccflags-y += -I$(dxipc_src_top)/../vdec

ccflags-$(CONFIG_XTENSA_PLATFORM_D13VDEC) += -DCHIP_NAME_d13
ccflags-$(CONFIG_XTENSA_PLATFORM_D14VDEC) += -DCHIP_NAME_d14

