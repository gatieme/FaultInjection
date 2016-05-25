/*
*  Author: HIT CS HDMC team.
*  Create: 2010-3-12 8:50
*  Last modified: 2010-6-13 14:06:20
*  Description:
*  	Memory fault injection engine running as a kernel module.
*		This module will create "/proc/memoryEngine/" directory and 9 proc nodes.
*   Write paramenters and request to these proc nodes and read the output from related proc node.
*/

#include "memoryEngine.h"

/*
*	proc entries
*
struct proc_dir_entry *dir = NULL;                  /// the directory of the MEMORY INJECT Moudle
struct proc_dir_entry *proc_pid = NULL;				/// write only
struct proc_dir_entry *proc_va = NULL;				/// write only
struct proc_dir_entry *proc_ctl = NULL;				/// write only
struct proc_dir_entry *proc_kFuncName = NULL;	    /// write only
struct proc_dir_entry *proc_val = NULL;				/// rw
struct proc_dir_entry *proc_signal = NULL;		    /// rw
struct proc_dir_entry *proc_pa = NULL;				/// read only
struct proc_dir_entry *proc_taskInfo = NULL;    	/// read only
*/


/*
* proc node values
*
int             pid;								/// pid
unsigned long   va;					                /// virtual  Address
unsigned long   pa;					                /// physical Addreess
int             ctl;								/// ctl
int             signal;								/// signal
char            kFuncName[MAX_LINE];                /// kFuncName
long            memVal;							    /// memVal

unsigned long   ack_pa;			                    /// physical Address
unsigned long   ack_va;			                    /// virtual  Address
int             ack_signal;						    /// signal
int             ret;                                /// return value
char            taskInfo[PAGE_SIZE];	            /// taskInfo

///////////////////////////////////////////////

unsigned long   userspace_phy_mem;                  /// user space physical memory
long            orig_pa_data;                       /// origin data of the physics memory
long            new_pa_data;                        /// the new data you want write to physics memory

int             faultInterval;
*/


extern struct proc_dir_entry    *proc_pid;				/// write only

extern int                      pid;								/// pid
/*
*
*/
int proc_write_pid( struct file *file,
                    const char __user *buffer,
                    unsigned long count,
                    void * data)
{
	int iRet;
	char sPid[MAX_LINE];

	if(count <= 0)
    {
        return FAIL;
    }

    memset(sPid, '\0', sizeof(sPid));
    /////////////////////////////////////////////////////////////////////
    //
    //  copy_from_user函数的目的是从用户空间拷贝数据到内核空间，
    //  失败返回没有被拷贝的字节数，成功返回0.
    //  这么简单的一个函数却含盖了许多关于内核方面的知识,
    //  比如内核关于异常出错的处理.
    //  从用户空间拷贝数据到内核中时必须很小心,
    //  假如用户空间的数据地址是个非法的地址,或是超出用户空间的范围，
    //  或是那些地址还没有被映射到，都可能对内核产生很大的影响，
    //  如oops，或被造成系统安全的影响.
    //  所以copy_from_user函数的功能就不只是从用户空间拷贝数据那样简单了，
    //  他还要做一些指针检查连同处理这些问题的方法.
    //
    //  函数原型在[arch/i386/lib/usercopy.c]中
    //  unsigned long
    //  copy_from_user( void *to,
    //                  const void __user *from,
    //                  unsigned long n)
    //
    /////////////////////////////////////////////////////////////////////
    //
    //  将用户空间中, 地址buffr指向的count个数据拷贝到内核空间地址sPid中
    iRet = copy_from_user(sPid, buffer, count);
	if(iRet != 0)
    {
        dbginfo("Error when copy_from_user...\n");
        return FAIL;
    }

    iRet = sscanf(sPid, "%d", &pid);        //  将读出来的数据sPid赋值给模块的全局变量pid
	if(iRet != 1)
    {
        return FAIL;
    }
	dbginfo("Rcv pid:%d\n",pid);

    return count;
}

