BIN = ../../workspace/
all: getpinfo v2p readpa writepa
	
getpinfo: getTaskInfo.cpp
	g++ -o $@ $^
v2p: vir_to_phy.cpp
	g++ -o $@ $^
readpa: read_phy_mem.cpp
	g++ -o $@ $^
writepa: write_phy_mem.cpp
	g++ -o $@ $^
install:
	cp getpinfo $(BIN)
	cp v2p $(BIN)
	cp readpa $(BIN)
	cp writepa $(BIN)
	cp inject_sys_process.sh $(BIN)
clean: 
	rm getpinfo v2p readpa writepa
