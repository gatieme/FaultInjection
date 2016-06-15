#########################################################################
# File Name: kill.sh
# Author: GatieMe
# mail: gatieme@163.com
# Created Time: Thu 27 Oct 2016 01:02:30 PM CST
#########################################################################
#!/bin/bash


all=`pidof hello`
for process in $all
do
    echo $process
    kill $process
done
