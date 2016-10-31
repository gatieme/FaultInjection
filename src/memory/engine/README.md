#1  工具介绍
-------


##1.1   **关于本模块**
-------

本模块作为故障注入工具的驱动模块, 负责如下功能

*   获取进程的task_struct的该信息

*   负责将给定的虚拟地址空间(pid + 虚拟地址)转换为物理地址


##1.2   **关于本工具**
-------

本工具最早完成于2010年针对安腾架构架构实现,

已基本完成了功能, 但是当时所有的工作都在用户空间通过读写/proc/pid来完成

因为在2016年的更新中, 由gatieme对工具进行了重写, 主要增加了如下功能

1.  对工具进行解耦才操作, 划分为应用层, 工具层, 驱动层.

2.  实现了一个通用的内核驱动proc模块, 替代原来/proc/pid用户空间读写方式 

3.  对应用层的改动比较小, 主要是解耦和增加功能(故障注入次数和支持内核栈故障注入)



##1.3   关于**文件戳**
-------

由于此工具的初版完成时间较早, 期间也经过几代人的努力

鄙人认为, 所遇人的努力都应该得到重视, 

因此文件的开头保留了文件戳, 谨以此来感谢所有为本项目做过贡献的师兄和网友

```cpp
/*
*  Author: gatieme @ HIT CS HDMC team.
*  Create: 2016-3-12 8:50
*  Last modified: 2010-6-13 14:06:20
*  Description:
*
*   Our project was completed in 2010 @ v1.0.0
*   This module is create by gatieme @2015-11-29
*
*  	Memory fault injection engine running as a kernel module.
*	This module will create "/proc/memoryEngine/" directory and 9 proc nodes.
*   Write paramenters and request to these proc nodes and read the output from related proc node.
*/
```

#2  获取进程的pcb信息
-------

##2.1   说明
-------

|  可执行程序 | 源代码 | 脚本 | 描述 |
| ------------- |:-------------:| -----:| -----:|
| getpinfo | getTaskInfo.cpp |  getpinfo.sh | 获取进程的task结构信息 |

其实其本质是执行了如下信息

##2.2   执行操作
-------

```c
# 写入将要读取的进程pid
echo pid > /proc/memoryEngine/pid

#  写入控制标识REQUEST_GETTASKINFO = 1
echo 1 > /proc/memoryEngine/ctl

# 读取信号量, 信号量为1时说明读入完成, 否则则等待

cat /proc/memoryEngine/signal
# 读取进程的task信息

cat /proc/memoryEngine/taskInfo
```

##2.3   执行
-------

**二进制文件执行**

-p执行进程号, 获取进程号为pid的进程信息
`
``c
./getpinfo -p pid
```

-e指定可执行程序, 我们将通过execve加载可执行程序, 并读取其信息
```c
./getpinfo -e ./hello
```

**脚本执行**
```c
sh getpinfo.sh pid
```

##2.4   执行结果

![获取进程的pcb信息](./images/get_task_info.jpg)



#3  驱动问题
-------


##3.1   mmap返回错误"Operation not permitted!"
-------


###3.1.1    问题发现
-------

在driver中通过alloc_pages申请得到的page，将page的物理地址export到user space
但是user space拿到这个物理地址后并不能mmap成功

通过perror("mmap")，发现总是返回错误"Operation not permitted!"

后来发现是由于kernel对user space访问/dev/mem是有限制的，

通过编译选项：CONFIG_STRICT_DEVMEM来限制user space

对物理内存的访问，这个选项的说明在arch/x86/Kconfig.debug中有说明：



###3.1.2    问题解析
-------

```cpp
config STRICT_DEVMEM
    bool "Filter access to /dev/mem"
        ---help---
        If this option is disabled, you allow userspace (root) access to all
        of memory, including kernel and userspace memory. Accidental
        access to this is obviously disastrous, but specific access can
        be used by people debugging the kernel. Note that with PAT support
        enabled, even in this case there are restrictions on /dev/mem
        use due to the cache aliasing requirements.
        
        If this option is switched on, the /dev/mem file only allows
        userspace access to PCI space and the BIOS code and data regions.
        This is sufficient for dosemu and X and all common users of /dev/mem.                                                            
        If in doubt, say Y.
```


只有在.config文件中设置`CONFIG_STRICT_DEVMEM=n`才能获得对整个memory的访问权限

在默认情况下，`CONFIG_STRICT_DEVMEM=y`, 这也就是之前mmap总是报错：“Operation not permitted”的原因。
                                                                          
                                                                          
设置这个选项后，编译kernel，然后运行tool, mmap还是返回错误："Invalid argument".
后来查到还需要设置编译选项`CONFIG_X86_PAT=n`, 这个选项也是默认开启的, 
但是要关闭这个选项还需要开启CONFIG_EXPERT，否则CONFIG_X86_PAT总是关不掉。
                                                                     
设置好这三个编译选项后，重新编译kernel，然后运行tool，发现kernel已经解除了对mmap的访问限制，
可以正确读取对应物理地址的内容了


###3.1.3    问题解决
-------



可以使用如下命令查看系统的配置信息


```cpp
cat /usr/src/linux-headers-`uname -r`/.config |  grep -E "CONFIG_STRICT_DEVMEM|CONFIG_X86_PAT"
```


所以，如果要使用mmap映射/dev/mem文件的话，必须将这两个量取消


*   如果不取消CONFIG_STRICT_DEVMEM, 则/dev/mem不允许映射, mmap报错"Operation not permitted"的原因


*   如果不取消CONFIG_X86_PAT, 则内核空间不能映射, 调用mmap()的时候会出现"Invalid Parameter"错误



在.config文件中设置：

```cpp
CONFIG_X86_PAT=n  
CONFIG_STRICT_DEVMEM=n  
```

或者在以下路径中设置取消两个选项：


*   CONFIG_X86_PAT的位置在：  

```cpp
Processor type and features  —>  
[*]   x86 PAT support  
```

*   CONFIG_STRICT_DEVMEM的位置在：  

```cpp
Kernel hacking  —>  
[ ] Filter access to /dev/mem   
```



