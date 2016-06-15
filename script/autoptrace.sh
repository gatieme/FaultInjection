RESULT_DIR=./ptrace/RESULT
mkdir -p $RESULT_DIR

for (( NUM = 10; NUM <= 10; NUM += 50 ))
do
    mkdir -p $RESULT_DIR/$NUM
    echo "positions        types         non   sigalrm   sigsegv   sigill   sigbus" >> $RESULT_DIR/$NUM/result.log
    for POS in  PT_F33 PT_F33 PT_F34 PT_F35 PT_F36 \
                PT_F37 PT_F38 PT_F39 PT_F40 PF_F41 \
                PF_F42 PT_F43 PT_F44 PT_F45 PT_F46 \
                PF_F47 PT_F48 PT_F49 PT_F50 PT_F51 \
                PF_F52 PT_F53 PT_F54 PT_F55 PT_F56 \
                PF_F57 PT_F58 PT_F59 PT_F60 PT_F61 \
                PF_F62 PT_F63 PT_F64 PT_F65 
    do
        mkdir -p $RESULT_DIR/$NUM/$POS
        for TYPE in 1_bit_flip 2_bit_flip 1_bit_0 2_bit_0 1_bit_1 2_bit_1
        do
            echo -e "$POS $TYPE $NUM"
            for (( i = 1; i <= $NUM; i++))
            do
                echo -e "$POS-$TYPE-$NUM Test Num : $i"
                echo -e "$POS-$TYPE-$NUM Test Num : $i" >> $RESULT_DIR/$NUM/$POS/$POS-$TYPE-$NUM.log
	            #../bin/hello &
                ../bin/ptrace/injector --step 100000 --mode microsecond --pos $POS --type $TYPE -e ../bin/ptrace/hello >> $RESULT_DIR/$NUM/$POS/$POS-$TYPE-$NUM.log
            done
            python ./readlog.py -u ptrace -p $POS -t $TYPE -n $NUM >> $RESULT_DIR/$NUM/result.log
        done
    done
done
