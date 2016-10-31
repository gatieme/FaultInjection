MemoryFaultInjection
=======


#BUG描述
-------


##bug-001
-------

**编号** ： bug-001

**描述** ： 地址空间修改失败

**详细信息**

执行sudo ./memInjector -l stack -m random -t word_0 -p 1
将进程1的栈区域某块地址随机清0的时候失败

![bug-001](./bug-001.jpg)

**问题定位**

未知

##bug-002
-------


**编号** ： bug-001

**描述** ： -e指定加载可执行程序时, execv传入参数的问题

**详细信息**

执行sudo ./memInjector -l stack -m random -t word_0 -e ls -al
执行运行"ls -al"程序，对该程序栈区域某块地址随机清0的时候execv失败

![bug-002](./bug-002.jpg)

问题定位：
系统执行"ls -al"的方式是通过fork子进程, 然后execv来执行的
但是execv接收两个参数, 1--可执行程序路径, 2--可执行程序的参数
我们的问题就在于第2个参数传入的程序运行参数有问题


##bug-003
-------


编号：bug-003

描述：SIGSEG

详细信息：

```shell
workspace/hello 
sudo ./memInjector -l stack -m random -t word_0 -p `pidof hello`
```
出现SIGSEGV
![bug-003](./bug-003.jpg)
