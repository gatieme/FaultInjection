#!/bin/bash
#########################################################################
# File Name: getppid.sh
# Author: GatieMe
# mail: gatieme@163.com
# Created Time: 2016年05月19日 星期四 19时04分26秒
#########################################################################
#get_ppid.sh


pid=$1

if [ -z ${pid} ];then
    read -p "PID: " pid
fi
ppid=`ps -p ${pid:-$$} -o ppid=`
command=`ps -p ${pid:-$$} -o command=`
echo "pid    ppid    command"
echo "${pid}    ${ppid}    ${command}"


# The output of ps –ef is a list of user and kernel processes running on your system. 
# Kernel process names are surrounded by square brackets ([]).
if [[ "${command}" =~ \[*\] ]];then 
    echo " is a kernel thread"
else 
    echo " is a user process"
fi;
