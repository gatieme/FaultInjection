#########################################################################
# File Name: bug2.sh
# Author: gatieme
# Created Time: 2016年04月19日 星期二 16时16分29秒
#########################################################################
#!/bin/bash

       
#   when getTaskMMinfo error                                                      
#   在故障注入的时候获取taskInfo的时候, 遇到意外情况，导致链接终端                
 

#   2016-04-19 16:12                                                              
#   find bugs when write CTL                                                      
#   you can see this bugs for do the next thing                                   
    
cd ../../tools/
../../injector/top &
./getpinfo 4430
