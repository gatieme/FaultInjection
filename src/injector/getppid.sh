#########################################################################
# File Name: getppid.sh
# Author: GatieMe
# mail: gatieme@163.com
# Created Time: 2016年05月19日 星期四 19时04分26秒
#########################################################################
#!/bin/bash
#get_ppid.sh


pid=$1

if [ -z $pid ];then
    read -p "PID: " pid
fi
ps -p ${pid:-$$} -o ppid=
