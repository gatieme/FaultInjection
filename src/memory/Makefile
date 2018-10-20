ROOT=../..
SRC=..
BIN=$(ROOT)/bin/
MEMORY_BIN=$(ROOT)/bin/memory
PTRACE_BIN=$(ROOT)/bin/ptrace
UNIXBENCH_BIN=$(ROOT)/bin/unixbench

MEMORY_SRC=$(SRC)/memory
PTRACE_SRC=$(SRC)/ptrace
UNIXBENCH_SRC=$(SRC)/unixbench

#PLATFORM=$(shell $(SRC)/systype.sh)
PLATFORM=linux
include $(SRC)/Make.defines.$(PLATFORM)
include $(SRC)/Make.readme


# director config
MEMORY_ENGINE=$(MEMORY_SRC)/engine
MEMORY_INJECTOR=$(MEMORY_SRC)/injector

DIRS=$(MEMORY_ENGINE) $(MEMORY_INJECTOR)


MAKE_DIR=make -C

SUBDIRS=$(DIRS)

.PHONY:all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
		$(MAKE_DIR) $@

#all	:
#	for i in $(DIRS); do \
#		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
#	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

install :
	for i in $(DIRS); do \
		(cd $$i && echo "install $$i" && $(MAKE) install) || exit 1; \
	done

github:
	make clean
	cd $(ROOT)/..
	git add -A
	git commit -m $(GITHUB_COMMIT)
	git push origin master

insmod:
	cd $(ENGINE)&& make insmod

reinsmod:
	cd $(ENGINE)&& make reinsmod
