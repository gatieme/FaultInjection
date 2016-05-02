BIN = ../../workspace/


targets = devmem devkmem getjiff mem

all:$(targets)

	
devmem:devmem.o
	gcc -o $@ $^

devkmem:devkmem.o
	gcc -o $@ $^

getjiff:getjiff.o
	gcc -o $@ $^

mem:mem.o
	gcc -o $@ $^

%.o:%.cpp
	g++ -o $@ -c $^

%.o:%.c
	gcc -o $@ -c $^

install:
	cp getpinfo $(BIN)
	cp v2p $(BIN)
	cp readpa $(BIN)
	cp writepa $(BIN)
	cp inject_sys_process.sh $(BIN)
clean:
	rm -rf *.o 
	rm -rf $(targets)
