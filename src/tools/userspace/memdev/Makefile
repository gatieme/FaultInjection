ROOT=../../..
BIN=$(ROOT)/bin
MEMORY_BIN=$(BIN)/memory
PTRACE_BIN=$(BIN)/ptrace
#PLATFORM=$(shell $(ROOT)/systype.sh)
#include $(ROOT)/Make.defines.$(PLATFORM)


targets = devmem devkmem mem getjiff

all:$(targets)

devmem: devmem.o
	g++ -o $@ $^
devkmem: devkmem.o
	g++ -o $@ $^
mem: mem.o
	g++ -o $@ $^
getjiff: getjiff.o
	g++ -o $@ $^

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
