#!/usr/bin/python
# encoding=utf-8


#!coding:utf-8

import re
import sys
import urllib2
import argparse
import xlrd
import commands
import os
import subprocess



positions = ""
types = ""
numbers = ""
def readFile(file):
    """
    """
    file_object = open(file)

    try :

        read_data = file_object.read( )
        return read_data

    finally :

        file_object.close( )



def writeData(positions, types, nums, sigalrm, sigsegv, sigill) :
    """
    """
    resultfile = "./RESULT/" + nums + "/result.log"
    file_object = open(resultfile, "a")
    file_object.write(positions.center(10) + types.center(20) + str(sigalrm).center(10) + str(sigsegv).center(10) + str(sigill).center(10) + "\n")
    file_object.close( )





def getCode(positions, types, data) :
    """
    """
    try:
        #匹配的信息如下
        #[ 2016-1-26 23:25:52]Process 18497 exited with code 0
        #[  2016-1-26 23:26:5]Process 18556 termed with signal 11(SIGSEGV)
        reStr = r'.*?(\d{4}-\d{1,2}-\d{1,2} \d{1,2}:\d{1,2}:\d{1,2})]Process (\d{1,5}) (exited with code \d|termed with signal \d{1,2}\((.*?)\))'
        #item[0] -=> [ 2016-1-26 23:25:52]
        #item[1] -=> pid
        #item[2] -=> Process 18497 exited with code 0 | Process 18556 termed with signal 11(SIGSEGV)
        #item[3] -=> "" | SIGSEGV

        non_exception = 0
        sigsegv_exception = 0
        sigill_exception = 0
        sigalrm_exception = 0
        pattern = re.compile(reStr, re.S)
        myItems = re.findall(pattern, data)
        #print len(myItems)

        #print myItems
        for item in myItems:
        	#print item
        	if item[3] == "" :
        		non_exception += 1
        	elif item[3] == "SIGALRM" :
        		sigalrm_exception += 1
        	elif item[3] == "SIGILL" :
        		sigill_exception += 1
        	elif item[3] == "SIGSEGV" :
        		sigsegv_exception += 1
        #print "SIGSEGV", sigsegv_exception
        #print "SIGILL", sigill_exception
        #print "SIGALRM", sigalrm_exception
        print positions.center(10), types.center(20), str(sigalrm_exception).center(10), str(sigsegv_exception).center(10), str(sigill_exception).center(10)

        return (sigalrm_exception, sigsegv_exception, sigill_exception)

    finally:
    	pass





if __name__ == "__main__" :
    # 测试正则表达式

    reload(sys)
    sys.setdefaultencoding("utf-8")
    if len(sys.argv) > 1:               #  如果在程序运行时，传递了命令行参数
        pass
        #  打印传递的命令行参数的信息
        #print "您输入的所有参数共 %d 个，信息为 sys.argv = %s" % (len(sys.argv), sys.argv)

        #for i, eachArg in enumerate(sys.argv):
        #    print "[%d] = %s" % (i, eachArg)
    else:
    	print "Useage : read.py file..."
    	exit(0)

    parser = argparse.ArgumentParser( )
    parser.add_argument("-f", "--file", dest = "resultfile", help = "The file you want to read...")
    parser.add_argument("-p", "--position", dest = "positions", help = "The file you want to read...")
    parser.add_argument("-t", "--type", dest = "types", help = "The file you want to read...")
    parser.add_argument("-n", "--num", dest = "numbers", help = "The file you want to read...")
    args = parser.parse_args( )

    resultfile = "./RESULT/" + args.numbers + "/" +args.positions + "/" + args.positions + "-" + args.types + "-" + args.numbers + ".log"

    #print resultfile
    resultdata = readFile(resultfile)
    (sigalrm, sigsegv, sigill) = getCode(args.positions, args.types, resultdata)
    #writeData(args.positions, args.types, args.numbers, sigalrm, sigsegv, sigill)
    pass

