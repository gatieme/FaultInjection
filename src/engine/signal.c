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

extern struct proc_dirnentry    *proc_signal;		    /// rw
extern int                      ack_signal;						    /// signal
extern int                      signal;								/// signal
/*
*
*/
int proc_read_signal(   char * page,
                        char **start,
                        off_t off,
                        int count,
                        int * eof,
                        void * data)
{
	int iLen;
    dbginfo("%d\n", ack_signal);
    iLen = sprintf(page, "%d", ack_signal);

    return iLen;
}

/*
*
*/
int proc_write_signal(  struct file *file,
                        const char *buffer,
                        unsigned long count,
                        void * data)
{
	int iRet;
	char sSignal[MAX_LINE];

	if(count <= 0)
    {
        return FAIL;
    }

    memset(sSignal, '\0', sizeof(sSignal));
	iRet = copy_from_user(sSignal, buffer, count);
	if(iRet)
    {
        return FAIL;
    }

    iRet = sscanf(sSignal,"%d",&signal);
	if(iRet != 1)
    {
        return FAIL;
    }

    dbginfo("Rcv signal:%d\n",signal);

    return count;
}

