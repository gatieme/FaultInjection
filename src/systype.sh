#########################################################################
# File Name: systype.sh
# Author: GatieMe
# mail: ftcl@163.com
# Created Time: Tue 26 Jan 2016 11:17:24 PM CST
#########################################################################
#!/bin/bash


case `uname -s` in
"FreeBSD")
    PLATFORM="freebsd"
    ;;

"Linux")
    PLATFORM="linux"
    ;;

"Darwin")
    PLATFORM="macos"
    ;;
                        
"SunOS")
    PLATFORM="solaris"
    ;;

*)
    echo "Unknown platform" >&2
    exit 1
esac

echo $PLATFORM
exit 0
