MODULE_MAME = ipt_SKMARK

KDIR ?= /lib/modules/$(shell uname -r)/build
SOURCE_FILES = kmod/ipt_SKMARK.c
INCLUDE_DIR = -I$(src)/include -I$(KDIR)/include/uapi -I$(KDIR)/include 

ccflags-y := $(INCLUDE_DIR)
OBJS = $(SOURCE_FILES:.c=.o)
obj-m += $(MODULE_MAME).o
$(MODULE_MAME)-y = $(OBJS)

all:
			$(BUILD_VARS) make -C $(KDIR) M=$(PWD) modules
clean:
			$(BUILD_VARS) make -C $(KDIR) M=$(PWD) clean			 
