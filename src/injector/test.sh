#########################################################################
# File Name: test.sh
# Author: gatieme
# Created Time: 2016年05月06日 星期五 16时59分05秒
#########################################################################
#!/bin/bash

sudo rmmod memoryEngine
cd ../engine && make
sudo insmod ./memoryEngine.ko
cd ../injector
sudo ./memInjector -l stack -m random -t word_0 --time 1 --timeout 3 -p 1
