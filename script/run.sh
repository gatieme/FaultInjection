#########################################################################
# File Name: run.sh
# Author: gatieme
# Created Time: 2016年06月12日 星期日 15时24分26秒
#########################################################################
#!/bin/bash


if [ $1 == "memory" ]; then
    sudo ../bin/memory/memInjector -l stack -m random -t word_0 --time 10 --timeout 3 -e ../bin/memory/hello
elif [ $1 == "ptrace" ]; then
    sudo ../bin/ptrace/injector --step 100000 --mode microsecond --pos PT_F34 --type 2_bit_flip -e ../bin/ptrace/hello
fi






