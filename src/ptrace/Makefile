# director config
ROOT=$(shell pwd)/../..

SRC=$(ROOT)/src
MEMORY_SRC=$(SRC)/memory
PTRACE_SRC=$(SRC)/ptrace

BIN=$(ROOT)/bin
MEMORY_BIN=$(BIN)/memory
PTRACE_BIN=$(BIN)/ptrace

PLATFORM=$(shell $(SRC)/systype.sh)
include $(SRC)/Make.defines.$(PLATFORM)
include $(SRC)/Make.readme

EXE = injector
HELLO = hello
WORKLOAD = workload



OBJS = interface.o injector.o ptrace.o fault.o util.o

TARGET = $(EXE) $(WORKLOAD) $(HELLO)

all:$(TARGET)

$(EXE):$(OBJS)
	$(CPP) $(CPPFLAG) $^ -o $(EXE) -I./

$(WORKLOAD):workload.c
	$(CPP) $(CPPFLAG) $^ -o $(WORKLOAD) -I./


$(HELLO):hello.o
	$(CPP) $(CPPFLAG) $^ -o $@



%.o: %.cpp
	$(CPP) $(CPPFLAG) -c $^ -o $@ -I./ -std=c++11

%.o:%.c
	$(CC) $(CFLAG) -c $^ -o $@ -I./

clean:
	rm -rf	$(TARGET)
	rm -rf	*.o


install:
	mkdir -p $(BIN)
	mkdir -p $(PTRACE_BIN)
	cp $(EXE) $(PTRACE_BIN)/injector
	cp $(HELLO) $(PTRACE_BIN)/hello
	cp $(WORKLOAD) $(PTRACE_BIN)/wordload

test :
	sudo ./injector --step 100000 --mode microsecond --pos PT_F34 --type 2_bit_flip -e ./hello
