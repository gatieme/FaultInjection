# Makefile for the memory fault injection engine.
#
# Author: HIT CS HDMC team.
# Create: 2010-3-13 9:25:04
# Last modified: 2010-6-16 15:49:14
#

MEMORY_SRC=..
MEMORY_ENGINE_SRC=$(MEMORY_SRC)/engine
MEMORY_INJECTOR_SRC=$(MEMROY_SRC)/injector
MEMORY_TOOL_SRC=$(MEMORY_SRC)/tools

STACK_SMASHING_PROTECTOR =
#-fno-stack-protector
#-z execstack -mpreferred-stack-boundary=2 -ggdb

EXE = memInjector
GCC	= g++
CFLAG = -c -Wall $(DEFFLAGS) -I./  $(STACK_SMASHING_PROTECTOR)
LDFLAG = -g
BIN = $(ROOT)/../workspace
ENGINE = $(ROOT)/engine
TOOLS  = $(ROOT)/tools

all:main.o  memoryFault.o memoryInjectorTool.o memoryInjector.o memoryEngine.o  ptrace.o
	$(GCC) $(LDFLAG) -o $(EXE) $^
#	make -C $(ENGINE)
#	make -C $(TOOLS)

.cpp.o:
	$(GCC) $(CFLAG) $(LDFLAG) $<

install:
	cp ./memInjector $(BIN)/memInjector
	cp $(MEMORY_ENGINE)/memory-engine.ko $(BIN)/memory-engine.ko

debug :
	make clean
	make DEBUG=y

#ifeq ($(EXE),$(wildcard $(EXE)))
#EXISTED=$(wildcard $(EXE))
#ifeq ("$(EXIST)","")
test:
	sudo ./memInjector -l stack -m random -t word_0 --time 10 --timeout 3 -e ../tools/hello

clean:
#	make -C $(ENGINE) clean
#	make -C $(TOOLS) clean
	rm -rf *.o
	rm -rf $(EXE)


github:
	cd $(ROOT) && make github

insmod:
	cd $(MEMORY_ENGINE_SRC) && make insmod

reinsmod:
	cd $(MEMORY_ENGINE_SRC) && make reinsmod

#  Modify by gatieme for  **stack smashing detected** when execute.
#  http://blog.csdn.net/qustdjx/article/details/8022474
#  http://blog.csdn.net/white_eyes/article/details/7168062
#  -fno-stack-protector -z execstack -mpreferred-stack-boundary=2 -ggdb
