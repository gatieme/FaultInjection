# Makefile for the memory fault injection engine.
#
# Author: HIT CS HDMC team.
# Create: 2010-3-13 9:25:04
# Last modified: 2010-6-16 15:49:14
#
ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)


EXE = memInjector
GCC	= g++
CFLAG = -c -Wall $(DEFFLAGS) -I./ 
LDFLAG = -g
BIN = $(ROOT)/../workspace
ENGINE = $(ROOT)/engine
TOOLS  = $(ROOT)/tools

all:main.o memoryInjector.o memoryEngine.o memInjector.o ptrace.o
	$(GCC) $(LDFLAG) -o $(EXE) $^
#	make -C $(ENGINE)
#	make -C $(TOOLS)

.cpp.o:
	$(GCC) $(CFLAG) $(LDFLAG) $<

install:
	cp ./memInjector $(BIN)/memInjector
	cp $(ENGINE)/memoryEngine.ko $(BIN)/memoryEngine.ko

clean:
#	make -C $(ENGINE) clean
#	make -C $(TOOLS) clean
	rm *.o
	rm $(EXE)
