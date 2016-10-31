CPP		= g++
CPPFLAG = -g -Wall
MV = mv
RM = rm
CP = cp
EXE = injector
HELLO = hello
WORKLOAD = test



OBJS = interface.o injector.o ptrace.o fault.o util.o

TARGET = $(EXE) $(WORKLOAD) $(HELLO)

all:$(TARGET)

$(EXE):$(OBJS)
	$(CPP) $(CPPFLAG) $^ -o $(EXE) -I./

$(WORKLOAD):workload.c
	$(CPP) $(CPPFLAG) $^ -o $(WORKLOAD) -I./


$(HELLO):hello.o
	$(CPP) $(CPPFLAG) $^ -o $@

install:
	$(MV) $(EXE) ../bin/injector
	$(MV) $(HELLO) ../bin/hello
	cp ../test.conf ../bin/test.conf
	$(MV) $(WORKLOAD) ../workload/test
	make -C unixBench
	$(CP) ./unixBench/pgms/arithoh ../workload/arithoh
	#$(CP) ./unixBench/pgms/context1 ../workload/context1
	#$(CP) ./unixBench/pgms/dhry2 ../workload/dhry2
	#$(CP) ./unixBench/pgms/dhry2reg ../workload/dhry2reg
	#$(CP) ./unixBench/pgms/double ../workload/double
	#$(CP) ./unixBench/pgms/float ../workload/float
	#$(CP) ./unixBench/pgms/fsbuffer ../workload/fsbuffer
	#$(CP) ./unixBench/pgms/fsdisk ../workload/fsdisk
	#$(CP) ./unixBench/pgms/fstime ../workload/fstime
	#$(CP) ./unixBench/pgms/hanoi ../workload/hanoi
	#$(CP) ./unixBench/pgms/int ../workload/int
	#$(CP) ./unixBench/pgms/long ../workload/long
	#$(CP) ./unixBench/pgms/looper ../workload/looper
	#$(CP) ./unixBench/pgms/pipe ../workload/pipe
	#$(CP) ./unixBench/pgms/register ../workload/register
	#$(CP) ./unixBench/pgms/short ../workload/short
	#$(CP) ./unixBench/pgms/spawn ../workload/spawn
	#$(CP) ./unixBench/pgms/syscall ../workload/syscall



#sudo ./injector --step 100000 --mode microsecond --pos PT_F34 --type 2_bit_flip -c ../conf/default.conf  -e ./hello
run :
	sudo ./injector --step 100000 --mode microsecond --pos PT_F34 --type 2_bit_flip -e ./hello


%.o: %.cpp %.h
	$(CPP) $(CPPFLAG) -c $< -I./ -std=c++11

clean:
	$(RM) -f *.o $(TARGET)
	$(RM) -f ../bin/*
	$(RM) -f ../workload/*
	$(RM) -f ../result/*
	$(RM) -f ../faultTable/*
	make -C unixBench clean
