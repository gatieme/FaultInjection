BIN = ../../workspace/


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
	gcc -o %@ $^

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
