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
*/
/*
extern struct proc_dir_entry *proc_va;				/// write only
extern struct proc_dir_entry *proc_ctl;				/// write only
extern struct proc_dir_entry *proc_kFuncName;	    /// write only
extern struct proc_dir_entry *proc_val;				/// rw
extern struct proc_dir_entry *proc_signal;		    /// rw
extern struct proc_dir_entry *proc_pa;				/// read only
extern struct proc_dir_entry *proc_taskInfo;    	/// read only
*/

extern struct proc_dir_entry *proc_ctl;				/// write only

/*
int             pid;								/// pid
unsigned long   va;					                /// virtual  Address
unsigned long   pa;					                /// physical Addreess
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

extern struct proc_dir_entry    *proc_ctl;				/// write only
extern int                      ctl;								/// ctl
/*
*
*/
int proc_write_ctl( struct file *file,
                    const char *buffer,
                    unsigned long count,
                    void * data)
{
	int iRet;
	char sCtl[MAX_LINE];

	if(count <= 0)
    {
        return FAIL;
    }

    memset(sCtl, '\0', sizeof(sCtl));

    iRet = copy_from_user(sCtl, buffer, count);
	if(iRet)
    {
        return FAIL;
    }

    iRet = sscanf(sCtl,"%d",&ctl);
	if(iRet != 1)
    {
        return FAIL;
    }

    do_request();

    return count;
}

const struct file_operations proc_ctl_fops =
{
    .owner = THIS_MODULE,
    //.read  = proc_read_ctl,                       // can read
    .write = proc_write_ctl,                        // write only
};
