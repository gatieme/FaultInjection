#########################################################################
# File Name: read_vir_addr.sh
# Author: gatieme
# Created Time: 2016年05月27日 星期五 11时37分42秒
#########################################################################
#!/bin/bash

pid=$1
va=$2
echo "pid = ${pid}, va = ${va}"

# virtual to physical
#res=$`./v2p ${pid} ${va}`
sudo ./v2p ${pid} ${va} > res.txt
#echo "res = ${res}"
pa=`awk 'END{print}' res.txt`
echo "va = ${va} to pa = ${pa}"

sudo ./readpa ${pa}
rm res.txt
