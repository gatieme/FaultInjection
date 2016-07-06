# Makefile for the memory fault injection engine.
#
# Author: HIT CS HDMC team.
# Create: 2010-3-12 8:46
# Last modified: 2010-6-13 13:55:29
# Description:
# 	This program is loaded as a kernel(v2.6.18 or later) module.
# 	Use "make install" to load it into kernel.
# 	Use "make remove" to remove the module out of kernel.
#
ROOT=..
#PLATFORM=$(shell $(ROOT)/systype.sh)
#include $(ROOT)/Make.defines.$(PLATFORM)


ifneq ($(KERNELRELEASE),)

#CFG_FLAGS += -O2  -I./
#EXTRA_CFLAGS  += $(C_FLAGS) $(CFG_INC) $(CFG_INC)  


MODULE_NAME := memory-engine

RESMAIN_CORE_OBJS := memoryEngine.o 
RESMAIN_GLUE_OBJS := ctl.o	physicalAddr.o	signal.o		\
					 virtualAddr.o 		kFuncName.o			\
					 memVal.o	pid.o	taskInfo.o


memory-engine-objs := $(RESMAIN_GLUE_OBJS) $(RESMAIN_CORE_OBJS)   
obj-m := memory-engine.o

else

MODULE_NAME := memory-engine
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#KERNELDIR ?= /home/gatieme/Work/Kernel/linux-headers-3.14-desktop
PWD := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	make -C $(KERNELDIR) M=$(PWD) modules_install



insmod:
	sudo insmod $(MODULE_NAME).ko

reinsmod:
	sudo rmmod $(MODULE_NAME)
	sudo insmod $(MODULE_NAME).ko

github:
	cd $(ROOT) && make github

rmmod:
	sudo rmmod $(MODULE_NAME)

test :
	sudo ../injector/memInjector -l stack -m random -t word_0 --time 1 --timeout 3 -p 1

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f modules.order Module.symvers Module.markers

.PHNOY:
	modules modules_install clean



endif  
  
