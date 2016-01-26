# Makefile for the memory fault injection engine.
#
# Author: HIT CS HDMC team.
# Create: 2010-3-13 9:25:04
# Last modified: 2010-6-16 15:49:14
#
EXE = memInjector
GCC	= g++
CFLAG = -c -Wall  -I./
LDFLAG = -g
BIN = ../workspace
ENGINE = ./memoryEngine
TOOLS  = ./tools

all:memoryInjector.o memoryEngine.o memInjector.o ptrace.o
	$(GCC) $(LDFLAG) -o $(EXE) $^
	make -C $(ENGINE)
#	make -C $(TOOLS)
.cpp.o:
	$(GCC) $(CFLAG) $(LDFLAG) $<
install:
	cp memInjector $(BIN)/memInjector
	cp $(ENGINE)/memoryEngine.ko $(BIN)/memoryEngine.ko
clean:
	make -C $(ENGINE) clean
#	make -C $(TOOLS) clean
	rm *.o
	rm $(EXE)
