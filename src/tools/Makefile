BIN = ../../workspace/


target = getpinfo v2p readpa writepa devmem devkmem

all:$(target)

	
getpinfo: getTaskInfo.o
	g++ -o $@ $^
v2p: vir_to_phy.o
	g++ -o $@ $^
readpa: read_phy_mem.o
	g++ -o $@ $^
writepa: write_phy_mem.o
	g++ -o $@ $^

devmem:devmem.o
	gcc -o $@ $^

devkmem:devkmem.o
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
	rm *.o 
	rm getpinfo v2p readpa writepa
