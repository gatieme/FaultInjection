ROOT=../../..
BIN=$(ROOT)/bin
MEMORY_BIN=$(BIN)/memory
PTRACE_BIN=$(BIN)/ptrace
#PLATFORM=$(shell $(ROOT)/systype.sh)
#include $(ROOT)/Make.defines.$(PLATFORM)


targets = getpinfo v2p readpa writepa iskthread hello

all:$(targets)
	cd ./memdev && make

getpinfo: getTaskInfo.o
	g++ -o $@ $^
v2p: vir_to_phy.o
	g++ -o $@ $^
readpa: read_phy_mem.o
	g++ -o $@ $^
writepa: write_phy_mem.o
	g++ -o $@ $^
iskthread: is_kernel_thread.o
	gcc -o $@ $^

hello:hello.o
	gcc -o $@ $^

%.o:%.cpp
	g++ -o $@ -c $^

%.o:%.c
	gcc -o $@ -c $^

install	:
	mkdir -p $(BIN)
	mkdir -p $(MEMORY_BIN)
	cp $(targets) $(MEMORY_BIN)

clean:
	rm -rf *.o
	rm -rf $(targets)

github:
	cd $(ROOT) && make github

insmod:
	cd $(ENGINE) && make insmod

reinsmod:
	cd $(ENGINE) && make reinsmod
