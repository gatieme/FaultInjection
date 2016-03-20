MemoryFaultInjection
=======

#配置安装
------

##目录结构
-------


# 编译修改
-------

##  内核目录的问题
-------

编译驱动使用了uname -r来获取内核头文件的地址
如果linux系统升过级请，自动为内核头文件建立链接
sudo ln -s 2.6.32-573.18.1.el6.x86_64 2.6.32-504.el6.x86_64

## error: 'struct proc_dir_entry' has no member named 'owner'
-------
vim  /usr/src/kernels/2.6.32-504.el6.x86_64/include/linux/proc_fs.h
error: 'struct proc_dir_entry' has no member named 'owner'的解决方法
在编译Android 4.0驱动的时候，使用到了proc_dir_entry结构体中的owner成员，但是编译的时候，却出现了下面的错误：
drivers/hello/hello.c:217: error: 'struct proc_dir_entry' has no member named 'owner'
make[2]: *** [drivers/hello/hello.o] 错误 1
make[1]: *** [drivers/hello] 错误 2
make: *** [drivers] 错误 2

由错误信息可以看出struct proc_dir_entry结构体中没有找到owner的成员。
 
看到引用的proc_fs.h头文件，发现里面的struct proc_dir_entry结构体中，的确没有owner成员，
在该结构体中添加以下代码：
	struct module *owner;

保存后，重新编译，OK了。
proc_fs.h文件在kernel源码下的include/linux目录下面

#地址选项
-------

系统进程代码段(BUG)
系统进程数据段
系统进程栈段
用户进程代码段(BUG)
用户进程数据段
用户进程栈段

#虚拟地址
-------

随机random
**其他方式未实现**


#故障注入类型
-------
one_bit_0       一位置0
one_bit_1       一位置1
one_bit_flip    一位反转
word_0          word清0
page_0          一页清


