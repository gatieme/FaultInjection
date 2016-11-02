RESULT_DIR=RESULT
mkdir -p $RESULT_DIR

for (( NUM = 400; NUM <= 1000; NUM += 50 ))
do
    mkdir -p $RESULT_DIR/$NUM
    echo "positions        types         non   sigalrm   sigsegv   sigill   sigbus" >> $RESULT_DIR/$NUM/result.log
    for POS in stack data text kstack
    do
        mkdir -p $RESULT_DIR/$NUM/$POS
        for TYPE in one_bit_0 one_bit_1 one_bit_flip word_0 word_flip
        do
            echo -e "$POS $TYPE $NUM"
            for (( i = 1; i <= $NUM; i++))
            do
                echo -e "$POS-$TYPE-$NUM Test Num : $i"
                echo -e "$POS-$TYPE-$NUM Test Num : $i" >> $RESULT_DIR/$NUM/$POS/$POS-$TYPE-$NUM.log
	            #../bin/hello &
                ../bin/memInjector --pos $POS --mode random  --type $TYPE --time 10 --timeout 3 -e ../bin/hello >> $RESULT_DIR/$NUM/$POS/$POS-$TYPE-$NUM.log
            done
            python ./readlog.py -p $POS -t $TYPE -n $NUM >> $RESULT_DIR/$NUM/result.log
        done
    done
done
