PROGRAM = main

EXTRA_COMPONENTS = \
	extras/http-parser \
	extras/dhcpserver \
	$(abspath ./components/wifi_config) \
	$(abspath ./components/cJSON) \
	$(abspath ./components/wolfssl) \
	$(abspath ./components/homekit)

FLASH_SIZE ?= 32

EXTRA_CFLAGS += -DHOMEKIT_SHORT_APPLE_UUIDS

include $(SDK_PATH)/common.mk

monitor:
	$(FILTEROUTPUT) --port $(ESPPORT) --baud 115200 --elf $(PROGRAM_OUT)
