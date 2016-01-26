#!/bin/bash
i=1
while [ 1 ]
do
	echo -n $i
	echo -n  " "
	ps aux | grep 'init \[3\]'
	sleep 1
	i=$(($i+1))
done
exit 0
