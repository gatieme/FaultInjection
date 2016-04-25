#ROOT=.
#PLATFORM=$(shell $(ROOT)/systype.sh)
#include $(ROOT)/Make.defines.$(PLATFORM)



DIRS=src



all:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE)) || exit 1; \
	done
clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

install :
#	for i in $(DIRS); do \
#		(cd $$i && echo "install $$i" && $(MAKE) install) || exit 1; \
#	done
	cp $(INJECTOR)/memInjector $(BIN)/memInjector
	cp $(ENGINE)/memoryEngine.ko $(BIN)/memoryEngine.ko
