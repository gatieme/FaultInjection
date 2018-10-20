#!/bin/bash

#getTaskInfo print
#0	unsigned long total;
#1	unsigned long locked;
#2	unsigned long shared;
#3	unsigned long exec;
#4	unsigned long stack;
#5	unsigned long reserve;
#6	unsigned long def_flags;
#7	unsigned long nr_ptes;
#8	unsigned long start_code;
#9	unsigned long end_code;
#10	unsigned long start_data;
#11	unsigned long end_data;
#12	unsigned long start_brk;
#13	unsigned long brk;
#14	unsigned long start_stack;
#15	unsigned long arg_start;
#16	unsigned long arg_end;
#17	unsigned long env_start;
#18	unsigned long env_end;

function printHexNum(){
	printf "0x"
	printf "%x" $1;
}
function printDebug()
{
	echo -e $1;
}

start_code=8
end_code=9
start_data=10
end_data=11
start_brk=12
brk=13
start_stack=14

STACK_SIZE=$((16#1000))

if [ $# -ne 2 ]; then
	echo -e "Useage:./ingect_sys_process pid position"
	echo -e "position: data/code/stack"
	exit 0;
fi
pid=$1
position=$2

#./getpinfo pid
Info=$(cat taskinfo)
i=0
for item in $Info
do
	TaskInfo[$i]=$item
	printDebug "$i\t$item"
	i=$(($i+1))
done

if [ ${#TaskInfo[@]} -ne 19 ]; then
	echo -e "Pid is not valid"
	exit 0
fi
echo
if [ $position == "data" ]; then
	start_pa=$((16#${TaskInfo[start_data]}))
	end_pa=$((16#${TaskInfo[end_data]}))
	printHexNum $start_pa
	printHexNum $end_pa
elif [ $position == "code" ]; then
	start_pa=$((16#${TaskInfo[start_code]}))
	end_pa=$((16#${TaskInfo[end_code]}))
	printHexNum $start_pa
	printHexNum $end_pa
elif [ $position == "stack" ]; then
	end_pa=$((16#${TaskInfo[start_stack]}))
	start_pa=$(($end_pa-$STACK_SIZE))
	printHexNum $start_pa
	printHexNum $end_pa
else
	echo -e "position is not valid"
	exit 0
fi
echo 
r=$((16#$RANDOM))
roffset=$(($r%($end_pa-$start_pa)))
injectVA=$(($start_pa+$roffset))
injectPA=$(./v2p $injectVA)

echo -n "Inject fault at "
		printHexNum $injectVA
echo -n "(physical: "
		printHexNum $injectPA
echo -e ")"
origNum=$(./readpa $injectPA)
echo -n "word_0("
		printHexNum $origNum
echo -e " -> 0x0)"
$(./writepa $injectPA 0)
exit 0
