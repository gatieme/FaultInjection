# director config
ROOT=$(shell pwd)/..
NORMAL_SRC=./normal
MEMDEV_SRC=./memdev
BIN=$(ROOT)/bin/


DIRS=$(NORMAL_SRC) $(MEMDEV_SRC)
MAKE_DIR=make -C

SUBDIRS=$(DIRS)

.PHONY:all $(SUBDIRS)

all: $(SUBDIRS) hello

$(SUBDIRS):
		$(MAKE_DIR) $@

#all	:
#	for i in $(DIRS); do \
#		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
#	done

clean:
	rm -rf *.o hello 
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

install :
	for i in $(DIRS); do \
		(cd $$i && echo "install $$i" && $(MAKE) install) || exit 1; \
	done

hello:hello.o
	gcc -o $@ $^
%.o:%.c
	gcc -o $@ -c $^

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
