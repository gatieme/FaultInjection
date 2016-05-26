/*
*  Author: HIT CS HDMC team.
*  Create: 2010-3-12 8:50
*  Last modified: 2010-6-13 14:06:20
*  Description:
*  	Memory fault injection engine running as a kernel module.
*		This module will create "/proc/memoryEngine/" directory and 9 proc nodes.
*   Write taskInforamenters and request to these proc nodes and read the output from related proc node.
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
struct proc_dir_entry *proc_taskInfo = NULL;				/// read only
struct proc_dir_entry *proc_taskInfo = NULL;    	/// read only
*/



/*
* proc node values
*
int             pid;								/// pid
unsigned long   va;					                /// virtual  Address
unsigned long   taskInfo;					                /// physical Addreess
int             ctl;								/// ctl
int             signal;								/// signal
char            kFuncName[MAX_LINE];                /// kFuncName
long            memVal;							    /// memVal

unsigned long   ack_taskInfo;			                    /// physical Address
unsigned long   ack_va;			                    /// virtual  Address
int             ack_signal;						    /// signal
int             ret;                                /// return value
char            taskInfo[taskInfoGE_SIZE];	            /// taskInfo

///////////////////////////////////////////////

unsigned long   userstaskInfoce_phy_mem;                  /// user space physical memory
long            orig_taskInfo_data;                       /// origin data of the physics memory
long            new_taskInfo_data;                        /// the new data you want write to physics memory

int             faultInterval;
*/



extern struct proc_dir_entry    *proc_pid;				/// write only

extern char                     taskInfo[PAGE_SIZE];	            /// taskInfo



#ifndef  PROC_SEQ_FILE_OPERATIONS


/*
*
*/
int proc_read_taskInfo(char * page,char **start, off_t off, int count, int * eof,void * data)
{
	int iLen;
	iLen = sprintf(page, "%s", taskInfo);
	return iLen;
}

const struct file_operations taskInfo_fops =
{
    .owner = THIS_MODULE,
    .read  = proc_read_taskInfo,                    // read only
    //.write = proc_write_taskInfo,                        // write only
};

#else

// seq_operations -> show
static int seq_show_taskInfo(struct seq_file *m, void *v)
{
	//char buf[MAX_LINE];
	int ret = 0;

    //ret = sprintf(buf, "%ls", ack_taskInfo);

	seq_printf(m, "%s", taskInfo);

	return 0; //!! must be 0, or will show nothing T.T
}


// seq_operations -> open
static int proc_open_taskInfo(struct inode *inode, struct file *file)
{
	return single_open(file, seq_show_taskInfo, NULL);
}

const struct file_operations proc_taskInfo_fops =
{
	.owner		= THIS_MODULE,
	.open		= proc_open_taskInfo,
	.read		= seq_read,
	//.write 		= proc_write_taskInfo,
	.llseek		= seq_lseek,
	.release	= single_release,

};

#endif
