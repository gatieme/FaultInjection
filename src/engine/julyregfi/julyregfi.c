#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/random.h>

#define PERMISSION	0644
#define MAX_LINE 256
#define jpnum 2
#define do_fork_id 0
#define do_timer_id 1

bool useit[jpnum]={1,1};  //if true then register probe

struct proc_dir_entry *dir = NULL;
struct proc_dir_entry *proc_aim = NULL;
struct proc_dir_entry *proc_fault = NULL;
struct proc_dir_entry *proc_time = NULL;
struct proc_dir_entry *proc_id = NULL;
struct proc_dir_entry *proc_signal = NULL;

int i=0,time=0,aim=0,fault=0,id=0,signal=0,fork_count=0,scan_count=0;
u64 rando;
char temp[MAX_LINE],date[MAX_LINE];

void getrando(int n)
{
	get_random_bytes(&rando, sizeof(u64));
	rando = (((int)rando)%n+n)%n;
}

struct my_pt_regs {
        unsigned long bx;
        unsigned long cx;
        unsigned long dx;
        unsigned long si;
        unsigned long di;
        unsigned long bp;
        unsigned long ax;
        unsigned long ds;
        unsigned long es;
        unsigned long fs;
        unsigned long gs;
        unsigned long orig_ax;
        unsigned long ip;
        unsigned long cs;
        unsigned long flags;
        unsigned long sp;
        unsigned long ss;
};


void change_reg(struct my_pt_regs *regs)
{
    getrando(32);
    if(fault & 1)
    {
        printk("Fortune: change reg ax from 0x%lx to ", regs->ax);
        regs->ax ^= (1 << rando);
        printk("0x%lx\n",regs->ax);
    }
    if(fault & 2)
    {
        printk("Fortune: change reg bx from 0x%lx to ",regs->bx);
        regs->bx ^= (1 << rando);
        printk("0x%lx\n",regs->bx);
    }
    if(fault & 4)
    {
        printk("Fortune: change reg cx from 0x%lx to ",regs->cx);
        regs->cx ^= (1 << rando);
        printk("0x%lx\n",regs->cx);
    }
    if(fault & 8)
    {
        printk("Fortune: change reg dx from 0x%lx to ",regs->dx);
        regs->dx ^= (1 << rando);
        printk("0x%lx\n",regs->dx);
    }
    if(fault & 16)
    {
        printk("Fortune: change reg si from 0x%lx to ",regs->si);
        regs->si ^= (1 << rando);
        printk("0x%lx\n",regs->si);
    }
	if(fault & 32)
    {
        printk("Fortune: change reg di from 0x%lx to ",regs->di);
        regs->di ^= (1 << rando);
        printk("0x%lx\n",regs->di);
    }
    if(fault & 64)
    {
        printk("Fortune: change reg bp from 0x%lx to ",regs->bp);
        regs->bp ^= (1 << rando);
        printk("0x%lx\n",regs->bp);
    }
    if(fault & 128)
    {
        printk("Fortune: change reg ds from 0x%lx to ",regs->ds);
        regs->ds ^= (1 << rando);
        printk("0x%lx\n",regs->ds);
    }
    if(fault & 256)
    {
        printk("Fortune: change reg es from 0x%lx to ",regs->es);
        regs->es ^= (1 << rando);
        printk("0x%lx\n",regs->es);
    }
    if(fault & 512)
    {
        printk("Fortune: change reg fs from 0x%lx to ",regs->fs);
        regs->fs ^= (1 << rando);
        printk("0x%lx\n",regs->fs);
    }
    if(fault & 1024)
    {
        printk("Fortune: change reg gs from 0x%lx to ",regs->gs);
        regs->gs ^= (1 << rando);
        printk("0x%lx\n",regs->gs);
    }
    if(fault & 2048)
    {
        printk("Fortune: change reg orig_ax from 0x%lx to ",regs->orig_ax);
        regs->orig_ax ^= (1 << rando);
        printk("0x%lx\n",regs->orig_ax);
    }
	if(fault & 4096)
    {
        printk("Fortune: change reg ip from 0x%lx to ",regs->ip);
        regs->ip ^= (1 << rando);
        printk("0x%lx\n",regs->ip);
    }
    if(fault & 8192)
    {
        printk("Fortune: change reg cs from 0x%lx to ",regs->cs);
        regs->cs ^= (1 << rando);
        printk("0x%lx\n",regs->cs);
    }
    if(fault & 16384)
    {
        printk("Fortune: change reg flags from 0x%lx to ",regs->flags);
        regs->flags ^= (1 << rando);
        printk("0x%lx\n",regs->flags);
    }
    if(fault & 32768)
    {
        printk("Fortune: change reg sp from 0x%lx to ",regs->sp);
        regs->sp ^= (1 << rando);
        printk("0x%lx\n",regs->sp);
    }
    if(fault & 65536)
    {
        printk("Fortune: change reg ss from 0x%lx to ",regs->ss);
        regs->ss ^= (1 << rando);
        printk("0x%lx\n",regs->ss);
    }
}

long my_do_fork(unsigned long clone_flags,unsigned long stack_start,struct pt_regs *regs,unsigned long stack_size,int __user *parent_tidptr,int __user *child_tidptr)
{
	if((!(aim & 1)) || signal == 0)
        jprobe_return();
	printk("Fortune: here.\n");
	if(time!=-1)
    {
        if(time > 0)
            --time;
        else
        {
            signal = 0;
            printk("Done.\n");
            fork_count = 0;
            jprobe_return();
        return 0;
        }
    }
	printk("Fortune: do_fork from %s fork_count %d\n",current->comm,++fork_count);
	//change_reg(regs);
	jprobe_return();
	return 0;
}

void scan_fi(void)
{
    struct task_struct *task,*p,*q;
    struct pt_regs *regs;
    struct list_head *pos;
    task = &init_task;

    printk("Fortune: scan_fi to %s(pid %d) \n", p->comm, p->pid);
    list_for_each(pos,&task->tasks)
    {
        if(time!=-1)
        {
            if(time > 0)
                --time;
            else
            {
                signal = 0;
                printk("Done.\n");
                scan_count = 0;
                jprobe_return();
                return;
            }
        }

        p = list_entry(pos, struct task_struct, tasks);
        printk("Fortune: scan_fi to %s(pid %d) count %d\n", p->comm, p->pid, ++scan_count);

        /*pt_regs * */regs = task_pt_regs(p);
        change_reg(/*my_pt_regs * */regs);

        list_for_each(pos,&p->tasks)
        {
            if(time!=-1) {if(time>0) --time; else {signal=0;printk("Done.\n");scan_count=0;jprobe_return();return;}}
            q=list_entry(pos, struct task_struct, tasks);
            printk("Fortune: scan_fi to %s(pid %d) count %d\n",q->comm,q->pid,++scan_count);
            regs = task_pt_regs(p);
            change_reg(regs);
        }
    }
    jprobe_return();
    return;
}

static int period_scanfi(struct kprobe *p, struct pt_regs *regs)
{
    if((aim & 2)&&signal)
        scan_fi();
    jprobe_return();
    return 0;
}

int proc_read_aim(char * page,char **start, off_t off, int count, int * eof,void * data)
{
	int iLen;
	iLen = sprintf(page, "%d", aim);
	return iLen;
}

int proc_write_aim(struct file *file,const char *buffer,unsigned long count,void * data)
{
	int iRet;
	if(count <= 0)
    {
        return -1;
    }
	memset(temp, '\0', sizeof(temp));
	iRet = copy_from_user(temp, buffer, count);
	if(iRet)
    {
        return -1;
    }

	iRet = sscanf(temp,"%d",&aim);

	if(iRet != 1)
    {
        return -1;
    }
	printk("Fortune: write aim:%d\n",aim);

    return count;
}

int proc_read_fault(char * page,char **start, off_t off, int count, int * eof,void * data)
{
		int iLen;
		iLen = sprintf(page, "%d", fault);
		return iLen;
}

int proc_write_fault(struct file *file,const char *buffer,unsigned long count,void * data)
{
		int iRet;
		if(count <= 0) { return -1; }
		memset(temp, '\0', sizeof(temp));
		iRet = copy_from_user(temp, buffer, count);
		if(iRet) { return -1; }
		iRet = sscanf(temp,"%d",&fault);
		if(iRet != 1) { return -1; }
		printk("Fortune: write fault:%d\n",fault);
		return count;
}

int proc_read_time(char * page,char **start, off_t off, int count, int * eof,void * data)
{
	int iLen;
	iLen = sprintf(page, "%d", time);
	return iLen;
}

int proc_write_time(struct file *file,const char *buffer,unsigned long count,void * data)
{
	int iRet;
	if(count <= 0) { return -1; }
	memset(temp, '\0', sizeof(temp));
	iRet = copy_from_user(temp, buffer, count);
	if(iRet) { return -1; }
	iRet = sscanf(temp,"%d",&time);
	if(iRet != 1) { return -1; }
	printk("Fortune: write time:%d\n",time);
	return count;
}

int proc_read_id(char * page,char **start, off_t off, int count, int * eof,void * data)
{
	int iLen;
	iLen = sprintf(page, "%d", id);
	return iLen;
}

int proc_write_id(struct file *file,const char *buffer,unsigned long count,void * data)
{
	int iRet;
	if(count <= 0) { return -1; }
	memset(temp, '\0', sizeof(temp));
	iRet = copy_from_user(temp, buffer, count);
	if(iRet) { return -1; }
	iRet = sscanf(temp,"%d",&id);
	if(iRet != 1) { return -1; }
	printk("Fortune: write id:%d\n",id);
	return count;
}

int proc_read_signal(char * page,char **start, off_t off, int count, int * eof,void * data)
{
	int iLen;
	iLen = sprintf(page, "%d", signal);
	return iLen;
}

int proc_write_signal(struct file *file,const char *buffer,unsigned long count,void * data)
{
	int iRet;
	if(count <= 0) { return -1; }
	memset(temp, '\0', sizeof(temp));
	iRet = copy_from_user(temp, buffer, count);
	if(iRet) { return -1; }
	iRet = sscanf(temp,"%d",&signal);
	if(iRet != 1) { return -1; }
	printk("Fortune: write signal:%d\n",signal);
	return count;
}

static struct jprobe jphyper[jpnum];

static int julyregfi_init(void)
{
	int ret;
	printk("julyregfi init.\n");

	if(useit[do_fork_id])
    {
        jphyper[do_fork_id].entry = (kprobe_opcode_t *)my_do_fork;
        jphyper[do_fork_id].kp.symbol_name="_do_fork";
    }

    if(useit[do_timer_id])
    {
        jphyper[do_timer_id].entry = (kprobe_opcode_t *)period_scanfi;
        jphyper[do_timer_id].kp.symbol_name="do_timer";
    }

    printk("Fortune: Begin register.\n");
	for(i = 0; i < jpnum; ++i)
	{
	    if(!useit[i])
            continue;
		jphyper[i].kp.addr=0;
		if((ret=register_jprobe(&jphyper[i])) < 0)
		{
			printk("Fortune: Error, register probe %d failed with return %d\n", i, ret);
			return -1;
		}
		if(jphyper[i].kp.addr == NULL)
		{
			printk("Fortune: Error, can't find %d address.\n", i);
			return -1;
		}
		printk("Fortune: Planted probe at %p, change %s\n", jphyper[i].kp.addr, jphyper[i].kp.symbol_name);
	}
	dir = proc_mkdir("julyregfi", NULL);
	if(dir == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi\n");
		return -1;
	}

#ifdef CREATE_PROC_ENTRY
    proc_aim = create_proc_entry("aim", PERMISSION, dir);

    if(proc_aim == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/aim\n");
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}

    proc_aim->read_proc = proc_read_aim;
	proc_aim->write_proc = proc_write_aim;

#elif defined PROC_CREATE
    const struct file_operations proc_aim_fops =
    {
        .owner = THIS_MODULE,
        .read  = proc_read_aim,                       // can read
        .write = proc_write_aim,                        // write only
    };
    proc_aim = proc_create("aim", PERMISSION, dir, &proc_aim_fops);
    if(proc_aim == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/aim\n");
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
#endif


#ifdef CREATE_PROC_ENTRY
	proc_fault = create_proc_entry("fault", PERMISSION, dir);
	if(proc_fault == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/fault\n");
		remove_proc_entry("aim",dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
	proc_fault->read_proc = proc_read_fault;
	proc_fault->write_proc = proc_write_fault;

#elif defined PROC_CREATE
    const struct file_operations proc_fault_fops =
    {
        .owner = THIS_MODULE,
        .read  = proc_read_fault,                       // can read
        .write = proc_write_fault,                        // write only
    };
    proc_fault = proc_create("fault", PERMISSION, dir, &proc_fault_fops);
	if(proc_fault == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/fault\n");
		remove_proc_entry("aim",dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
#endif



#ifdef CREATE_PROC_ENTRY
    proc_time = create_proc_entry("time", PERMISSION, dir);
	if(proc_time == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/time\n");
		remove_proc_entry("fault",dir);
		remove_proc_entry("aim",dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
	proc_time->read_proc = proc_read_time;
	proc_time->write_proc = proc_write_time;
#elif defined PROC_CREATE
    const struct file_operations proc_time_fops =
    {
        .owner = THIS_MODULE,
        .read  = proc_read_time,                       // can read
        .write = proc_write_time,                        // write only
    };
    proc_time = proc_create("time", PERMISSION, dir, &proc_time_fops);
	if(proc_time == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/time\n");
		remove_proc_entry("fault",dir);
		remove_proc_entry("aim",dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
#endif



#ifdef CREATE_PROC_ENTRY
    proc_id = create_proc_entry("id", PERMISSION, dir);
	if(proc_id == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/id\n");
		remove_proc_entry("time", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("aim", dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
	proc_id->read_proc = proc_read_id;
	proc_id->write_proc = proc_write_id;
#elif defined PROC_CREATE
    const struct file_operations proc_id_fops =
    {
        .owner = THIS_MODULE,
        .read  = proc_read_id,                       // can read
        .write = proc_write_id,                        // write only
    };
    proc_id = proc_create("id", PERMISSION, dir, &proc_id_fops);
	if(proc_id == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/id\n");
		remove_proc_entry("time", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("aim", dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
#endif



#ifdef CREATE_PROC_ENTRY
    proc_signal = create_proc_entry("signal", PERMISSION, dir);
	if(proc_signal == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/signal\n");
		remove_proc_entry("id", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("time", dir);
		remove_proc_entry("aim", dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
	proc_signal->read_proc = proc_read_signal;
	proc_signal->write_proc = proc_write_signal;
#elif defined PROC_CREATE
    const struct file_operations proc_signal_fops =
    {
        .owner = THIS_MODULE,
        .read  = proc_read_signal,                       // can read
        .write = proc_write_signal,                        // write only
    };
    proc_signal = proc_create("id", PERMISSION, dir, &proc_signal_fops);
	if(proc_signal == NULL)
	{
		printk("Fortune: Can't create /proc/julyregfi/signal\n");
		remove_proc_entry("id", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("time", dir);
		remove_proc_entry("aim", dir);
		remove_proc_entry("julyregfi", NULL);
		return -1;
	}
#endif

	printk("Fortune: Create /proc/julyregfi done.\n");
	return 0;
}

static void julyregfi_exit(void)
{
	printk("julyregfi exit.\n");
	for(i=0;i<jpnum;++i)
        if(useit[i])
		unregister_jprobe(&jphyper[i]);
	remove_proc_entry("aim", dir);
	remove_proc_entry("fault", dir);
	remove_proc_entry("time", dir);
	remove_proc_entry("id", dir);
	remove_proc_entry("signal", dir);
	remove_proc_entry("julyregfi", NULL);
}

module_init(julyregfi_init);
module_exit(julyregfi_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Little7");
