hardware_modules := gralloc hwcomposer audio nfc hwfm

ifeq ($(BOARD_HAVE_MTK_MT6620),true)
hardware_modules += gps
endif

include $(call all-named-subdir-makefiles,$(hardware_modules))
