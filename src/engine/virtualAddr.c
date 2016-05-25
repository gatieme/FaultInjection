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



extern struct proc_dir_entry    *proc_va;				/// write only
extern unsigned long            ack_va;			                    /// virtual  Address
extern unsigned long            va;					                /// virtual  Address
/*
*
*/
int proc_read_virtualAddr(  char * page,
                            char **start,
                            off_t off,
                            int count,
                            int * eof,
                            void * data)
{
	int iLen;

	iLen = sprintf(page, "%lx", ack_va);


    return iLen;
}

/*
*
*/
int proc_write_virtualAddr( struct file *file,
                            const char *buffer,
                            unsigned long count,
                            void * data)
{
	int iRet;
	char sVa[MAX_LINE];

	if(count <= 0)
    {
        return FAIL;
    }

    memset(sVa, '\0', sizeof(sVa));

    iRet = copy_from_user(sVa, buffer, count);
	if(iRet)
    {
        return FAIL;
    }

    iRet = sscanf(sVa,"%lx",&va);
	if(iRet != 1)
    {
        return FAIL;
    }

	dbginfo("Rcv virtual addr:0x%lx\n",va);

    return count;
}

