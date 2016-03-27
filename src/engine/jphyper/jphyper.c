#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
#include <linux/time.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/pgalloc.h>
#include <xen/evtchn.h>
#include <asm/hypervisor.h>



#include <xen/interface/io/blkif.h>
#include <xen/interface/io/xenbus.h>
#include <asm/xen/hypercall.h>
#include <asm/xen/page.h>


#if  0
#include <xen/blkif.h>
#include <xen/gnttab.h>
#include <xen/driver_util.h>
#include <xen/xenbus.h>
#endif
#include <xen/balloon.h>
#include <xen/evtchn.h>



#define pmd_val_ma(v) (v).pmd

#define PERMISSION	0644
#define MAX_LINE 256
#define jpnum 6
#define sys_ioctl_id 0
#define xen_pgd_pin_id 1
#define	xen_l2_entry_update_id 2
#define direct_remap_pfn_range_id 3
#define	xen_l3_entry_update_id 4
#define dispatch_rw_block_io_id 5

#ifdef __ia64__
#define XC_PAGE_SHIFT           14
#else
#define XC_PAGE_SHIFT           12
#endif
#define XC_PAGE_SIZE            (1UL << XC_PAGE_SHIFT)
#define BLK_RING_SIZE __CONST_RING_SIZE(blkif, XC_PAGE_SIZE)
#define MAX_REQUESTS            BLK_RING_SIZE

typedef struct privcmd_hypercall
{
	__u64 op;
	__u64 arg[5];
}privcmd_hypercall_t;



#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)
// modify by gatieme
typedef struct blkif_request        blkif_request_t;
typedef struct mmu_update           mmu_update_t;
typedef struct mmuext_op            mmuext_op_t;
#endif


typedef struct {
        blkif_request_t  req;
        struct blkif    *blkif;
        int              submitting;
        int              secs_pending;
        int16_t          status;
} pending_req_t;

typedef struct blkif {
        domid_t domid;
        long int handle;

        long int pdev;
        long int readonly;

        enum { DISCONNECTED, DISCONNECTING, CONNECTED } state;

        struct blkif_ops *ops;
        struct blkif *hash_next;

        void *prv;  /* device-specific data */
        void *info; /*Image parameter passing */
        pending_req_t pending_list[MAX_REQUESTS];
        int devnum;
        int fds[2];
        int be_id;
        int major;
        int minor;
        pid_t tappid;
        int drivertype;
        uint16_t cookie;
} blkif_t;

bool useit[jpnum]={1,1,1,1,1,1};  //if true then register probe

struct proc_dir_entry *dir = NULL;
struct proc_dir_entry *proc_aim = NULL;
struct proc_dir_entry *proc_fault = NULL;
struct proc_dir_entry *proc_time = NULL;
struct proc_dir_entry *proc_id = NULL;
struct proc_dir_entry *proc_signal = NULL;

int i=0,ioctlcount=0,last=-1,time=0,aim=-1,fault=-1,id=-1,signal=0,flag=0,leave=0,idtemp;
u64 rando;
char temp[MAX_LINE],date[MAX_LINE];
mmu_update_t tta,ttb;
mmu_update_t mmuupdateop;
mmuext_op_t mmuextop;

void getrando(int n)
{
	get_random_bytes(&rando, sizeof(u64));
	rando = (((int)rando)%n+n)%n;
}

void my_xen_l2_entry_update(pmd_t *ptr, pmd_t val)
{
	if(aim!=xen_l2_entry_update_id || signal==0) jprobe_return();
	if(leave==1) {BUG_ON(HYPERVISOR_mmu_update(&mmuupdateop, 1, NULL, DOMID_SELF) < 0);leave=0;}
	//if((strcmp(current->comm,"xm")==0)||(strcmp(current->comm,"xend")==0)) printk("Fortune: %s\n",current->comm); else jprobe_return();
	if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
	printk("Fortune: xen_l2_entry_update from %s\n",current->comm);
	printk("Fortune: pmd=0x%llx\tval=0x%llx\n",ptr->pmd,val.pmd);
	printk("Fortune: ptr real=0x%llx\n",virt_to_machine(ptr));
	mmuupdateop.ptr = virt_to_machine(ptr);
	mmuupdateop.val = pmd_val_ma(val);
	if(fault==0)
	{
		getrando(64);
		mmuupdateop.ptr ^= (1 << rando);
		printk("Fortune: change ptr to 0x%llx\n",mmuupdateop.ptr);
		leave=1;
	}
	if(fault==1)
	{
		getrando(64);
		ptr->pmd ^= (1 << rando);
		printk("Fortune: change pmd to 0x%llx\n",ptr->pmd);
		//not need to leave=1
	}
	if(fault==2)
	{
		getrando(64);
		mmuupdateop.val ^= (1 << rando);
		printk("Fortune: change val to 0x%llx\n",mmuupdateop.val);
		leave=1;
	}
	jprobe_return();
}

void my_xen_l3_entry_update(pud_t *ptr, pud_t val)
{
	if(aim!=xen_l3_entry_update_id || signal==0) jprobe_return();
	if(leave==1) {BUG_ON(HYPERVISOR_mmu_update(&mmuupdateop, 1, NULL, DOMID_SELF) < 0);leave=0;}
	if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
	printk("Fortune: xen_l3_entry_update from %s\n",current->comm);
	printk("Fortune: pud=0x%llx\tval=0x%llx\n",ptr->pgd.pgd,val.pgd.pgd);
	printk("Fortune: pud real=0x%llx\n",virt_to_machine(ptr));
	mmuupdateop.ptr = virt_to_machine(ptr);
	mmuupdateop.val = val.pgd.pgd;
	if(fault==0)
	{
		getrando(64);
		mmuupdateop.ptr ^= (1 << rando);
		printk("Fortune: change ptr to 0x%llx\n",mmuupdateop.ptr);
		leave=1;
	}
	if(fault==1)
	{
		getrando(64);
		ptr->pgd.pgd ^= (1 << rando);
		printk("Fortune: change pud to 0x%llx\n",ptr->pgd.pgd);
		//not need to leave=1
	}
	if(fault==2)
	{
		getrando(64);
		mmuupdateop.val ^= (1 << rando);
		printk("Fortune: change val to 0x%llx\n",mmuupdateop.val);
		leave=1;
	}
	jprobe_return();
}

void my_xen_pgd_pin(unsigned long ptr)
{
	if(aim!=xen_pgd_pin_id || signal==0) jprobe_return();
	if(leave==1) {HYPERVISOR_mmuext_op(&mmuextop, 1, NULL, DOMID_SELF);leave=0;}
	if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
	printk("Fortune: xen_pgd_pin from %s\n",current->comm);
	#ifdef CONFIG_X86_64
	mmuextop.cmd = MMUEXT_PIN_L4_TABLE;
	#elif defined(CONFIG_X86_PAE)
	mmuextop.cmd = MMUEXT_PIN_L3_TABLE;
	#else
	mmuextop.cmd = MMUEXT_PIN_L2_TABLE;
	#endif
	mmuextop.arg1.mfn = pfn_to_mfn(ptr >> PAGE_SHIFT);
	printk("Fortune: cmd=0x%x\tmfn=0x%lx\n",mmuextop.cmd,mmuextop.arg1.mfn);
	if(fault==0)
	{
		getrando(3);
		if(rando==0)
			mmuextop.cmd=MMUEXT_PIN_L4_TABLE;
		if(rando==1)
			mmuextop.cmd=MMUEXT_PIN_L3_TABLE;
		if(rando==2)
			mmuextop.cmd=MMUEXT_PIN_L2_TABLE;
		printk("Fortune: change cmd to 0x%x\n",mmuextop.cmd);
		leave=1;
	}
	if(fault==1)
	{
		getrando(32);
		mmuextop.arg1.mfn ^= (1 << rando);
		printk("Fortune: change mfn to 0x%lx\n",mmuextop.arg1.mfn);
		leave=1;
	}
	jprobe_return();
}

asmlinkage long my_sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{
	privcmd_hypercall_t hypercall;
	void __user *udata = (void __user *)arg;
	__u64 orig_op;
	if(aim!=sys_ioctl_id || signal==0) jprobe_return();
	//getdate();
	if(cmd==0x305000)	//IOCTL_PRIVCMD_HYPERCALL
	{
		if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
		++ioctlcount;
		printk("Fortune: changehyper from %s count %d\n", current->comm, ioctlcount);
		if(copy_from_user(&hypercall, udata, sizeof(hypercall)))
		{
			printk("Fortune: A copy_from_user error\n");
			jprobe_return();
		}
		orig_op = hypercall.op;
		//if(orig_op==36) {jprobe_return();return 0;}
		if(orig_op==26)
		{
			mmuext_op_t op26;
			op26.cmd=(*((mmuext_op_t *)hypercall.arg[0])).cmd;
			op26.arg1.mfn=(*((mmuext_op_t *)hypercall.arg[0])).arg1.mfn;
			op26.arg1.linear_addr=(*((mmuext_op_t *)hypercall.arg[0])).arg1.linear_addr;
			op26.arg2.nr_ents=(*((mmuext_op_t *)hypercall.arg[0])).arg2.nr_ents;
			op26.arg2.vcpumask=(*((mmuext_op_t *)hypercall.arg[0])).arg2.vcpumask;
			printk("cmd=%u\tmfn=0x%lx\tlinear_addr=0x%lx\tnr_ents=%u\tvcpumask=0x%lx\n",op26.cmd,op26.arg1.mfn,op26.arg1.linear_addr,op26.arg2.nr_ents,op26.arg2.vcpumask);
			if(fault==0)
			{
				getrando(16);
				(*((mmuext_op_t *)hypercall.arg[0])).cmd=rando;
				printk("Fortune: change cmd from %d to %d\n",op26.cmd,(*((mmuext_op_t *)hypercall.arg[0])).cmd);
			}
			if(fault==1)
			{
				getrando(32);
				(*((mmuext_op_t *)hypercall.arg[0])).arg1.mfn ^= (1 << rando);
				printk("Fortune: change mfn from 0x%lx to 0x%lx\n",op26.arg1.mfn,(*((mmuext_op_t *)hypercall.arg[0])).arg1.mfn);
			}
			if(fault==2)
			{
				getrando(32);
				(*((mmuext_op_t *)hypercall.arg[0])).arg1.linear_addr ^= (1 << rando);
				printk("Fortune: change linear_addr from 0x%lx to 0x%lx\n",op26.arg1.linear_addr,(*((mmuext_op_t *)hypercall.arg[0])).arg1.linear_addr);
			}
			if(copy_to_user(udata, &hypercall, sizeof(hypercall)))
			{
				printk("Fortune: B copy_to_user error\n");
				jprobe_return();
			}
		}
		printk("Fortune: orig_op=%llu\n",orig_op);
		for(i=0;i<5;++i)
		{
			printk("Fortune: arg[%d]=0x%llx\n", i, hypercall.arg[i]);
		}
		if(orig_op!=26 && orig_op==id)
        {
            if(fault==0)
			{
			    getrando(37);
				hypercall.op = rando;
				printk("Fortune: change cmd from %llu to %llu\n",orig_op,hypercall.op);
			}
			if(copy_to_user(udata, &hypercall, sizeof(hypercall)))
			{
				printk("Fortune: copy_to_user error\n");
				jprobe_return();
			}
        }
	}
	jprobe_return();
	return 0;
}

static void my_dispatch_rw_block_io(blkif_t * blkif, blkif_request_t * req, pending_req_t * pending_req)
{
    if(aim!=dispatch_rw_block_io_id || signal==0) jprobe_return();
	if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
	printk("Fortune: dispatch_rw_block_io from %s ",current->comm);
	printk("domid=%d\t%lld\t%d\n",blkif->domid,req->id,pending_req->blkif->domid);
	if(blkif->domid==id || id<=0)
	{
		if(fault==0)
		{
			getrando(3);
			if(rando==0)
			{
				idtemp=(int)req->id;
				req->id=(u64)pending_req->blkif->domid;
				pending_req->blkif->domid=idtemp;
			}
			if(rando==1)
			{
				idtemp=blkif->domid;
				blkif->domid=pending_req->blkif->domid;
				pending_req->blkif->domid=idtemp;
			}
			if(rando==2)
			{
				idtemp=blkif->domid;
				blkif->domid=(int)req->id;
				req->id=(u64)idtemp;
			}
			printk("Fortune: change domid to %d\t%lld\t%d\n",blkif->domid,req->id,pending_req->blkif->domid);
		}
		if(fault==1)
		{
			getrando(16);
			pending_req->status ^= (1 << rando);
			printk("Fortune: status change to %d\n",pending_req->status);
		}
		if(fault==2)
		{
			getrando(2);
			if(rando==0)
			{
				++req->nr_segments;
			}
			if(rando==1)
			{
				--req->nr_segments;
			}
			printk("Fortune: nr_segments change to %d\n",req->nr_segments);
		}
	}
	jprobe_return();
}

static int my_direct_remap_pfn_range(struct mm_struct *mm,unsigned long address, unsigned long mfn,unsigned long size, pgprot_t prot,domid_t  domid)
{
	if(aim!=direct_remap_pfn_range_id || signal==0) jprobe_return();
	if(time>0) --time; else {signal=0;printk("Done.\n");jprobe_return();}
	printk("Fortune: direct_remap_pfn_range from %s domid %d\n",current->comm,domid);
	if(domid==id || id<=0)
	{
		if(fault==0)
		{
			getrando(64);
			printk("Fortune: change pgd from 0x%llx ",mm->pgd->pgd);
			mm->pgd->pgd ^= (1 << rando);
			printk("to 0x%llx\n",mm->pgd->pgd);
		}
		if(fault==1)
		{
			getrando(32);
			printk("Fortune: change flag from 0x%lx ",mm->mmap_cache->vm_flags);
			mm->mmap_cache->vm_flags ^= (1 << rando);
			printk("to 0x%lx\n",mm->mmap_cache->vm_flags);
		}
	}
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
	if(count <= 0) { return -1; }
	memset(temp, '\0', sizeof(temp));
	iRet = copy_from_user(temp, buffer, count);
	if(iRet) { return -1; }
	iRet = sscanf(temp,"%d",&aim);
	if(iRet != 1) { return -1; }
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

static int mvmhyper_init(void)
{
	int ret;
	printk("mvmhyper init.\n");
	if(useit[sys_ioctl_id])
    {
        jphyper[sys_ioctl_id].entry = (kprobe_opcode_t *) my_sys_ioctl;
        jphyper[sys_ioctl_id].kp.symbol_name="sys_ioctl";
    }
    if(useit[xen_pgd_pin_id])
    {
        jphyper[xen_pgd_pin_id].entry = (kprobe_opcode_t *) my_xen_pgd_pin;
        jphyper[xen_pgd_pin_id].kp.symbol_name="xen_pgd_pin";
    }
	if(useit[xen_l2_entry_update_id])
    {
        jphyper[xen_l2_entry_update_id].entry = (kprobe_opcode_t *) my_xen_l2_entry_update;
        jphyper[xen_l2_entry_update_id].kp.symbol_name="xen_l2_entry_update";
    }
	if(useit[direct_remap_pfn_range_id])
    {
        jphyper[direct_remap_pfn_range_id].entry = (kprobe_opcode_t *) my_direct_remap_pfn_range;
        jphyper[direct_remap_pfn_range_id].kp.symbol_name="direct_remap_pfn_range";
    }
	if(useit[xen_l3_entry_update_id])
    {
        jphyper[xen_l3_entry_update_id].entry = (kprobe_opcode_t *) my_xen_l3_entry_update;
        jphyper[xen_l3_entry_update_id].kp.symbol_name="xen_l3_entry_update";
    }
	if(useit[dispatch_rw_block_io_id])
    {
        jphyper[dispatch_rw_block_io_id].entry = (kprobe_opcode_t *) my_dispatch_rw_block_io;
        jphyper[dispatch_rw_block_io_id].kp.symbol_name="dispatch_rw_block_io";
    }
    printk("Fortune: Begin register.\n");
	for(i=0;i<jpnum;++i)
	{
	    if(!useit[i])
            continue;
		jphyper[i].kp.addr=0;
		if((ret=register_jprobe(&jphyper[i]))<0)
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
	dir = proc_mkdir("jphyper", NULL);
	if(dir == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper\n");
		return -1;
	}
	proc_aim = create_proc_entry("aim", PERMISSION, dir);
	if(proc_aim == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper/aim\n");
		remove_proc_entry("jphyper", NULL);
		return -1;
	}
	proc_aim->read_proc = proc_read_aim;
	proc_aim->write_proc = proc_write_aim;
	proc_fault = create_proc_entry("fault", PERMISSION, dir);
	if(proc_fault == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper/fault\n");
		remove_proc_entry("aim",dir);
		remove_proc_entry("jphyper", NULL);
		return -1;
	}
	proc_fault->read_proc = proc_read_fault;
	proc_fault->write_proc = proc_write_fault;
	proc_time = create_proc_entry("time", PERMISSION, dir);
	if(proc_time == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper/time\n");
		remove_proc_entry("aim",dir);
		remove_proc_entry("fault",dir);
		remove_proc_entry("jphyper", NULL);
		return -1;
	}
	proc_time->read_proc = proc_read_time;
	proc_time->write_proc = proc_write_time;
	proc_id = create_proc_entry("id", PERMISSION, dir);
	if(proc_id == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper/id\n");
		remove_proc_entry("aim", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("time", dir);
		remove_proc_entry("jphyper", NULL);
		return -1;
	}
	proc_id->read_proc = proc_read_id;
	proc_id->write_proc = proc_write_id;
	proc_signal = create_proc_entry("signal", PERMISSION, dir);
	if(proc_signal == NULL)
	{
		printk("Fortune: Can't create /proc/jphyper/signal\n");
		remove_proc_entry("aim", dir);
		remove_proc_entry("fault", dir);
		remove_proc_entry("time", dir);
		remove_proc_entry("id", dir);
		remove_proc_entry("jphyper", NULL);
		return -1;
	}
	proc_signal->read_proc = proc_read_signal;
	proc_signal->write_proc = proc_write_signal;
	printk("Fortune: Create /proc/jphyper done.\n");
	return 0;
}

static void mvmhyper_exit(void)
{
	printk("mvmhyper exit.\n");
	for(i=0;i<jpnum;++i)
        if(useit[i])
		unregister_jprobe(&jphyper[i]);
	remove_proc_entry("aim", dir);
	remove_proc_entry("fault", dir);
	remove_proc_entry("time", dir);
	remove_proc_entry("id", dir);
	remove_proc_entry("signal", dir);
	remove_proc_entry("jphyper", NULL);
}

module_init(mvmhyper_init);
module_exit(mvmhyper_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Little7");

