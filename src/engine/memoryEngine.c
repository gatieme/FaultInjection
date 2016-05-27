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
struct proc_dir_entry *dir = NULL;                  /// the directory of the MEMORY INJECT Moudle
struct proc_dir_entry *proc_pid = NULL;				/// write only
struct proc_dir_entry *proc_va = NULL;				/// write only
struct proc_dir_entry *proc_ctl = NULL;				/// write only
struct proc_dir_entry *proc_kFuncName = NULL;	    /// write only
struct proc_dir_entry *proc_val = NULL;				/// rw
struct proc_dir_entry *proc_signal = NULL;		    /// rw
struct proc_dir_entry *proc_pa = NULL;				/// rw
struct proc_dir_entry *proc_taskInfo = NULL;    	/// read only

/*
* proc node values
*/
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


/*
 * export
 *
 */
EXPORT_SYMBOL(pa);
EXPORT_SYMBOL(taskInfo);
EXPORT_SYMBOL(ctl);
EXPORT_SYMBOL(ack_va);
EXPORT_SYMBOL(memVal);
EXPORT_SYMBOL(signal);
EXPORT_SYMBOL(va);
EXPORT_SYMBOL(ack_signal);
EXPORT_SYMBOL(kFuncName);
EXPORT_SYMBOL(pid);

EXPORT_SYMBOL(do_request);



/*
 * static function
 *
 * for warning: ‘jforce_sig_info’ declared ‘static’ but never defined
 */
static int handler_pre_kFunc(struct kprobe *p, struct pt_regs *regs);

static int jforce_sig_info(int sig,struct siginfo *info,struct task_struct *t);



/*
*	kprobe
*/
static struct kprobe kp_kFunc;

struct jprobe jprobe1 =
{
    .entry	= jforce_sig_info,
    .kp     =
    {
        .symbol_name = "force_sig_info",
    },
};

//时钟中断数计数
static int count = 0;

//保存原始代码
static long orig_code = 0;



/*
*  process the request
*/
void do_request(void)
{
	struct task_struct *task = NULL;
	unsigned long pa = 0;
	long kernel_va = 0;
	int status;

    int temp = 0;
	/// get a task's memory map information
	if(ctl == REQUEST_TASK_INFO)
	{
		dbginfo("Rcv request:Get task info\n");

        memset(taskInfo,'\0',sizeof(taskInfo));

        if(pid <= 0)
		{
			ack_signal = ACK_TASK_INFO;
			return;
		}
		task = findTaskByPid(pid);

        dprint("find task %p\n", task);

        if( task != NULL )
		{
			getTaskInfo(task, taskInfo, sizeof(taskInfo));
		    dbginfo("%s", taskInfo);
        }
        else
		{
			dbginfo("No such process\n");
        }
		ack_signal = ACK_TASK_INFO;

		return;
	}
	/// convert a process's linear address to physical address
	else if(ctl == REQUEST_V2P)
	{
		task = findTaskByPid(pid);
		if( task == NULL )
		{
			dbginfo("No such process\n");
			ack_pa = -1;
			ack_signal = ACK_V2P;
			return;
		}

        /*
         *  modify by gatieme @ 2016-05-27
         *  the kernel thread which run in kernel space have no user space
         *
         *  */
        if( task->mm == NULL )
		{
            dprint("the task %d:%s a kernel thread[task->mm == NULL]...\n", task->pid, task->comm);
			ack_pa = -1;
			ack_signal = ACK_V2P;
			return;
		}

		ack_pa = v2p(task->mm,va,&status);
		if(ack_pa == FAIL)
		{
			dbginfo("No physical address\n");
		}
		ack_signal = ACK_V2P;
		return;
	}
	/// convert kernel virtual address to physical address
	else if(ctl == REQUEST_KV2P)
	{
		ack_pa = kv2p(va,&status);
		if(pa == FAIL)
        {
			dbginfo("No physical address\n");
		}
		ack_signal = ACK_KV2P;
		return;
	}
	/// get kernel function's addr(kernel virtual address)
	else if(ctl == REQUEST_KFUNC_VA)
	{
		ack_va = kFunc2v(kFuncName);
		ack_signal = ACK_KFUNC_VA;
		return;
	}
	/// 请求读取内核函数起始地址内容
	else if(ctl == REQUEST_READ_KFUNC)
	{
		kernel_va = kFunc2v(kFuncName);
		memVal = *((long *)kernel_va);
		ack_signal = ACK_READ_KFUNC;
	}
	/// 请求改写内核函数起始地址内容
	else if(ctl == REQUEST_WRITE_KFUNC)
	{
		//利用kprobe，在第一次调用do_timer()时，注入故障
		int ret;
		count = 0;
		if(strlen(kFuncName) > 0)
		{
			faultInterval = 1;	//故障仅持续一个时钟周期
			kp_kFunc.addr = 0;
			kp_kFunc.symbol_name = kFuncName;
			kp_kFunc.pre_handler = handler_pre_kFunc;
			ret = register_kprobe(&kp_kFunc);
			if(ret < 0)
			{
				dbginfo("Fained to register kprobe\n");
				ack_signal = ACK_WRITE_KFUNC;
				return;
			}

			// 等待故障注入结束
			dbginfo("start count\n");
			temp = 0;

			while(1)
			{
				if(count == -1)
				{
					unregister_kprobe(&kp_kFunc);
					dbginfo("recovery\n");
					break;
				}
				if(temp == -1)
				{
					break;
				}
				temp++;
				//dbginfo("count:%d\n",count);
			}
		}
		ack_signal = ACK_WRITE_KFUNC;
		dbginfo("Success to inject MTTR fault\n");
		return;
	}
    /*
    *
    *  MODIFY  by gatieme @ 2016-05-26 19:39
    *  FOR     read and write physical address in kernel
    *
    *  #define REQUEST_READ_PA		10	    /// 请求获取全部物理内存信息
    *  #define REQUEST_WRITE_PA	11 	    /// 请求改写指定物理地址内容，改为用户态实现此功能
    *  #define REQUEST_MEM			12	    /// 请求获取全部物理内存信息
    *  #define REQUEST_ADDR_STOP	13	    ///
    */
    else if(ctl == REQUEST_READ_PA)
    {
        /*  get the physical address  */
		kernel_va = readpa(phy);
		memVal = *((long *)kernel_va);
		ack_signal = ACK_READ_PA;
    }
    else if(ctl == REQUEST_WRITE_PA)
    {
        /*  */
    }
}

struct dentry* file_entry(struct file *pfile)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)

    return pfile->f_path.dentry;

#else

    return pfile->f_dentry;

#endif

}

/*
*  get a task's memory map information
*/
int getTaskInfo(struct task_struct *pTask, char *pData, int length)
{
	struct mm_struct        *pMM;                   // struct mm_struct is defined in `include/linux/mm_types.h`
	struct vm_area_struct   *pVMA;
	struct vm_area_struct   *p;
	struct dentry *         pPath    = NULL;
	char                    *info    = pData;

    char                    file[MAX_LINE];
	char                    *end, *start;

	long                    phy_addr;
	unsigned long           start_va, end_va;
	int                     status;

	if(pTask == NULL) { return FAIL; }
	if((pMM = pTask->mm) == NULL)
    {
        dprint("the task %d:%s a kernel thread[task->mm == NULL]...\n", pTask->pid, pTask->comm);
        return FAIL;
    }

	memset(pData, '\0', length);


	//  前19个字段是关于进程内存信息的总体信息
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->total_vm, DELIMITER);   undbginfo("%lx%c", pMM->total_vm, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->locked_vm, DELIMITER);  undbginfo("%lx%c", pMM->locked_vm, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->shared_vm, DELIMITER);  undbginfo("%lx%c", pMM->shared_vm, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->exec_vm, DELIMITER);    undbginfo("%lx%c", pMM->exec_vm, DELIMITER);

	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->stack_vm, DELIMITER);   undbginfo("%lx%c", pMM->stack_vm, DELIMITER);


/// modify by gatieme for system porting NeoKylin-linux-3.14/16
#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 7, 0)
    //  error: ‘struct mm_struct’ has no member named ‘reserved_vm’
    //
    //  从linux 3.7.0开始内核不再支持RESERVED_VM
    //  struct mm_struct 也没有了reserved_mm字段
    //  struct vm_area_struct结构体中flag标志使用值 VM_RESERVED -=> (VM_DONTEXPAND | VM_DONTDUMP)
    //
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->reserved_vm, DELIMITER);undbginfo("%lx%c", pMM->reserved_vm, DELIMITER);
#endif

    safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->def_flags, DELIMITER);  undbginfo("%lx%c", pMM->def_flags, DELIMITER);

	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->nr_ptes, DELIMITER);    undbginfo("%lx%c", pMM->nr_ptes, DELIMITER);

	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->start_code, DELIMITER); undbginfo("%lx%c", pMM->start_code, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->end_code, DELIMITER);   undbginfo("%lx%c", pMM->end_code, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->start_data, DELIMITER); undbginfo("%lx%c", pMM->start_data, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->end_data, DELIMITER);   undbginfo("%lx%c", pMM->end_data, DELIMITER);

	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->start_brk, DELIMITER);  undbginfo("%lx%c", pMM->start_brk, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->brk, DELIMITER);        undbginfo("%lx%c", pMM->brk, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->start_stack, DELIMITER);undbginfo("%lx%c", pMM->start_stack, DELIMITER);

	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->arg_start, DELIMITER);  undbginfo("%lx%c", pMM->arg_start, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->arg_end, DELIMITER);    undbginfo("%lx%c", pMM->arg_end, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->env_start, DELIMITER);  undbginfo("%lx%c", pMM->env_start, DELIMITER);
	safe_sprintf(pData, length, info+strlen(info), "%lx%c", pMM->env_end, DELIMITER);    undbginfo("%lx%c", pMM->env_end, DELIMITER);


	pVMA = pMM->mmap;
	if(pVMA == NULL)
    {
        return OK;
    }
	for(p = pVMA; p != NULL; p = p->vm_next)
	{
		//  起始地址
		safe_sprintf(pData, length, info + strlen(info), "%lx %lx ", p->vm_start, p->vm_end);

        //  属性
		(p->vm_flags & VM_READ)   ? safe_sprintf(pData, length, info+strlen(info), "r") : safe_sprintf(pData, length, info+strlen(info), "-");

		(p->vm_flags & VM_WRITE)  ? safe_sprintf(pData, length, info+strlen(info), "w") : safe_sprintf(pData, length, info+strlen(info), "-");

		(p->vm_flags & VM_EXEC)   ? safe_sprintf(pData, length, info+strlen(info), "x") : safe_sprintf(pData, length, info+strlen(info), "-");

        (p->vm_flags & VM_SHARED) ? safe_sprintf(pData, length, info+strlen(info), "s") : safe_sprintf(pData, length, info+strlen(info), "p");


		//  对应文件名
		if(p->vm_file != NULL)
		{
            //  i find in linux-kernel-3.16
            //  http://lxr.free-electrons.com/source/include/linux/fs.h?v=3.16#L827
            //  struct path             f_path;
            //  #define f_dentry        f_path.dentry
            struct dentry *den = file_entry(p->vm_file);

            //if(p->vm_file->f_path.dentry != NULL)
            if(den != NULL)
            {
				safe_sprintf(pData, length, info+strlen(info), " ");
				memset(file,'\0',sizeof(file));
				//for(pPath = p->vm_file->f_path.dentry;
				for(pPath = den;
                    pPath != NULL;
                    pPath = pPath->d_parent)
				{

                    if(strcmp(pPath->d_name.name, "/") != 0)
					{
						strcpy(file + strlen(file), pPath->d_name.name);
						strcpy(file + strlen(file), "/");
						continue;
					}
					break;
				}
				do
				{
					end = file + strlen(file) - 1;
					for(start = end - 1; *start != '/' && start > file; start--);
					if(*start == '/')	{start++;}
					*end = '\0';

					safe_sprintf(pData, length, info+strlen(info), "/%s", start);
					*start = '\0';
				} while(start > file);
			}
		}
		safe_sprintf(pData, length, info+strlen(info), "%c", DELIMITER);

		//对应物理地址页
		start_va = p->vm_start;
		end_va = p->vm_end;
		while(end_va > start_va)
		{
			safe_sprintf(pData, length, info+strlen(info), "%lx-%lx\t", start_va, start_va + PAGE_SIZE);
			phy_addr = v2p(pMM, start_va, &status);
			if(phy_addr != FAIL)
			{
				safe_sprintf(pData, length, info+strlen(info), "va:0x%lx <--> pa:0x%lx", start_va, phy_addr);
			}
			start_va += PAGE_SIZE;
			safe_sprintf(pData, length, info+strlen(info), "%c", DELIMITER);
		}

		safe_sprintf(pData, length, info+strlen(info), "%c", DELIMITER);
	}

    dbginfo("get task info success...\n");
	return OK;
}

/*
*
*/
static int handler_pre_kFunc(struct kprobe *p, struct pt_regs *regs)
{
	unsigned long va;
	va = (unsigned long)p->addr;
	if(va <= 0) { return OK; }

	//第一次触发
	if(count == 0)
	{
		//读取前64字节，注入故障
		orig_code = *((long *)va);
		//_inject_fault(va,memVal);
		*((long *)va) = memVal;	//故障
		return OK;
	}
	count ++;
	//到达结束时间
	if(count == faultInterval + 1)
	{
		//恢复code
		*((long *)va) = orig_code;
		count = -1;
	}
	return OK;
}

/*
*	find_task_by_pid maybe not supported
*	O(n) is fine :)
*/
struct task_struct * findTaskByPid(pid_t pid)
{
	struct task_struct *task = NULL;

    //  Traversing the process in the system to find the PID
    //  add by gatieme @2016-03-20
    for_each_process(task)
	{
		if(task->pid == pid)
        {
            dbginfo("find task by pid = %d\n", pid);
			return task;
	    }
    }
	return NULL;
}

/*
* convert a process's linear address to physical address
*/
long v2p(struct mm_struct *pMM, unsigned long va, int *pStatus)
{
	pte_t *pte = NULL;
	unsigned long pa = FAIL;
    //dbginfo("");
	pte = getPte(pMM, va);
    //dbginfo("");
	if(pte != NULL)
	{
		pa = (pte_val(*pte) & PAGE_MASK) | (va & ~PAGE_MASK);
        //dbginfo("virt_addr 0x%lx in RAM is 0x%lx t .\n", va, pa);
        //dbginfo("contect in 0x%lx is 0x%lx\n", pa, *(unsigned long *)((char *)pa + PAGE_OFFSET));
        dbginfo("virtual : 0x%lx--(physical : 0x%lx)\n", va, pa);
	}
    else
    {
        dbginfo("virtual : 0x%lx to physical address error\n", va);
    }
    //printk("==========0x%lx\n", virt_to_phys(va));
	return pa;
}

/*
*  convert kernel virtual address to physical address
*
*/
long kv2p(unsigned long va,int *pStatus)
{
    if(va < 0)
    {
		return FAIL;
    }

    if(__pa(va) >= 0)
    {
		return __pa(va);
    }
    return FAIL;
}

/*
*  get kernel function's addr(kernel virtual address)
*	 the kernel function should be looked up in the System.map
*/
static struct kprobe kp;
long kFunc2v(char *funcName)
{
	int ret;
	unsigned long va;

	kp.addr = 0;
	kp.symbol_name = funcName;
	ret = register_kprobe(&kp);
	if(ret < 0)
	{
		dbginfo("Fained to register kprobe\n");
		return FAIL;
	}
	va = (unsigned long)kp.addr;
	unregister_kprobe(&kp);
	if(va == 0)
		return FAIL;
	return va;
}



/*
*
*/
struct vm_area_struct * getVMA(struct mm_struct *pMM,unsigned long va)
{
	struct vm_area_struct *p;
	if(pMM == NULL) return NULL;
	p = pMM->mmap;
	if(p == NULL) return NULL;

	for(; p != NULL; p = p->vm_next)
	{
		if( va >= p->vm_start && va < p->vm_end )
		{
			return p;
		}
	}
	return NULL;
}

/*
*
*/
pte_t * getPte(struct mm_struct *pMM, unsigned long va)
{
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;

    dprint("PGDIR_SHIFT = %d\n",  PGDIR_SHIFT);
    dprint("PUD_SHIFT = %d\n",    PUD_SHIFT);
    dprint("PMD_SHIFT = %d\n",    PMD_SHIFT);
    dprint("PAGE_SHIFT = %d\n",   PAGE_SHIFT);

    dprint("PTRS_PER_PGD = %d\n", PTRS_PER_PGD);
    dprint("PTRS_PER_PUD = %d\n", PTRS_PER_PUD);
    dprint("PTRS_PER_PMD = %d\n", PTRS_PER_PMD);
    dprint("PTRS_PER_PTE = %d\n", PTRS_PER_PTE);

    printk("PAGE_MASK = 0x%lx\n", PAGE_MASK);

    /*  判断给出的地址va是否合法(va < vm_end)  */
    if(!find_vma(pMM, va))
    {
        printk(KERN_INFO"virt_addr 0x%lx not available.\n", va);
        return NULL;
    }

    pgd = pgd_offset(pMM, va);
    dprint("pgd = 0x%p\n", pgd);
    dprint("pgd_val(*pgd) = 0x%lx\n", pgd_val(*pgd));
    if(pgd_none(*pgd))
    {
        printk(KERN_INFO"Not mapped in pgd.\n");
        return NULL;
    }

    pud = pud_offset(pgd, va);
    dprint("pud = 0x%p\n", pud);
    dprint("pud_val(*pud) = 0x%lx\n", pud_val(*pud));
    if(pud_none(*pud))
    {
        printk(KERN_INFO"Not mapped in pud.\n");
        return NULL;
    }

    pmd = pmd_offset(pud, va);
    dprint("pmd = 0x%p\n", pmd);
    dprint("pmd_val(*pmd) = 0x%lx\n", pmd_val(*pmd));
    if(pmd_none(*pmd))
    {
        printk(KERN_INFO"Not mapped in pmd.\n");
        return NULL;
    }

    /*  在这里，把原来的pte_offset_map( )改成了pte_offset_kernel  */
    /*  pte_offset_kernel(dir, addr)
     *
     *  Yields the linear address of the Page Table that corresponds
     *  to the linear address addr mapped by the Page Middle Directory dir.
     *  Used only on the master kernel page tables.
     *
     *  pte_offset_map(dir, addr)
     *
     *  Receives as parameters a pointer dir to a Page Middle Directory
     *  entry and a linear address addr;
     *  it yields the linear address of the entry in the Page Table
     *  that corresponds to the linear address addr.
     *  If the Page Table is kept in high memory,
     *  the kernel establishes a temporary kernel mapping,
     *  to be released by means of pte_unmap.
     *  The macros pte_offset_map_nested and pte_unmap_nested are identical,
     *  but they use a different temporary kernel mapping.
     */
    pte = pte_offset_map(pmd, va);

    //pte = pte_offset_kernel(pmd, va);
    dprint("pte = 0x%p\n", pte);
    dprint("pte_val(*pte) = 0x%lx\n", pte_val(*pte));
    dprint("*ppte = 0x%lx\n", pte_val(pte[-PTRS_PER_PTE]));
    if(pte_none(*pte))
    {
        printk(KERN_INFO"Not mapped in pte.\n");
        return NULL;
    }

    if(!pte_present(*pte))
    {
        printk(KERN_INFO"pte not in RAM.\n");
        return NULL;
    }
    pte_unmap(pte);
    return pte;
}


/*
*
*/
int setVMAFlags(struct mm_struct *pMM,unsigned long va,int *pStatus,int flags)
{
	struct vm_area_struct *p;
	p = getVMA(pMM,va);
	if(p == NULL) return FAIL;

	if(flags > 0)
	{
		p->vm_flags |= VM_WRITE;
		p->vm_flags |= VM_SHARED;
	}
	if(flags == 0)
	{
		p->vm_flags &= ~VM_WRITE;
		p->vm_flags &= ~VM_SHARED;
	}
	else { return FAIL; }
	return OK;
}

/*
*
*/
int setPageFlags(struct mm_struct *pMM,unsigned long va,int *pStatus,int flags)
{
	pte_t *pte = NULL;
	pte_t ret;
    //dbginfo("");
	pte = getPte(pMM, va);
    //dbginfo("");
	if( pte == NULL ) { return FAIL; }
	if(flags > 0)
	{
		ret = pte_mkwrite(*pte);
	}
	else if(flags == 0)
	{
		ret = pte_wrprotect(*pte);
	}
	else { return FAIL;	}
	return OK;
}

/*
 *  how to read and write
 *  a physical address or
 *  a virtual address of user process
 *  in kernel space
 *
 *  way 1
 *  map the physical address to kernel space, so that we can read and write
 *  see mmap, kmap or remap_pfn_range
 *
 *  way 2
 *  see copy_from_user
 *
 */
unsigned long readpa(unsigned long pa)
{
    unsigned long data = FAIL;

    /*unsigned long addr = (char *)kmap(pa);

    void volatile *mapStart = (void volatile *)kmap(pa);*/

    unsigned long va = phys_to_virt(pa);
    memcpy(data, va, sizeof(data));
    return data;
}

unsigned long writepa(unsigned long pa)
{

}



/*
 * map an virtual memory space `vma` to the kernel space `buffer`
 *
 */
unsigned long map_vma_to_kernel(unsigned char *buffer, struct vm_area_struct *vma)
{
    unsigned long page;
    unsigned char i;
    unsigned long start = (unsigned long)vma->vm_start;
    unsigned long end =  (unsigned long)vma->vm_end;

    unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);


    //  得到物理地址
    page = virt_to_phys(buffer);

    //  将用户空间的一个vma虚拟内存区映射到以page开始的一段连续物理页面上
    if(remap_pfn_range(vma, start, page>>PAGE_SHIFT, size, PAGE_SHARED))    //第三个参数是页帧号，由物理地址右移PAGE_SHIFT得到
    {
        return -1;
    }

    return page;
}

unsigned long readva(struct vm_area_struct *vma)
{
    static unsigned char *buffer = NULL;
    unsigned long page = FAIL;
    unsigned long data = FAIL;

    page = map_vma_to_kernel(buffer, vma);

    memcpy(vma, buffer, sizeof(unsigned long));

    return data;
}








/*
 * file_operations
 * RW read and write
 * RO read only
 * WO write only
 */
extern const struct file_operations proc_pa_fops;        /*  RW in physicalAddress.c */
extern const struct file_operations proc_va_fops;        /*  RW in virtualAddress.c  */
extern const struct file_operations proc_signal_fops;    /*  RW in signal.c          */
extern const struct file_operations proc_val_fops;       /*  RW in memVal.c          */

extern const struct file_operations proc_taskInfo_fops;  /*  RO in taskInfo.c        */

extern const struct file_operations proc_kFuncName_fops; /*  WO in taskInfo.c        */
extern const struct file_operations proc_pid_fops;       /*  WO in pid.c        */
extern const struct file_operations proc_ctl_fops;       /*  WO in ctl.c        */

/*
*  init memory fault injection module
*  初始化内存注入故障模块
*
*  使用proc_mkdir()创建一个dir = /proc/memoryEngine
*  再利用create_proc_read_entry()函数创建一个processinfo文件。
*  我们从模块里面获取的信息都将写入到这些文件中。
*
*/
static int __init initME(void)
{
	/*
     *  create a direntory named "memoryEngine" in /proc for the moudles
     *  as the interface between the kernel and the user program.
     *
     */
    dir = proc_mkdir("memoryEngine", NULL);
	if(dir == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/\n");
		return FAIL;
	}
    dbginfo("PROC_MKDIR ");
    printk("Create /proc/memoryEngine success...\n");

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 32)
    /// modify by gatieme for system porting NeoKylin-linux-3.14/16
    /// error: dereferencing pointer to incomplete type
	dir->owner = THIS_MODULE;
#endif


    /// modify by gatieme for system porting NeoKylin-linux-3.14/16 @ 2016--03-28 20:08
    /*
     *  ==
     *  write in STACK_OVER_FLOW http://stackoverflow.com/questions/26808325/implicit-declaration-of-function-create-proc-entry
     *  ==
     *
     *  proc filesystem has been refactored in 3.10,
     *  the function `create_proc_entry` has been removed,
     *  you should use the full featured `proc_create function` family.
     *
     *  Note that the signatures are different, you can find them in LXR
     *  3.10 version: http://lxr.free-electrons.com/source/include/linux/proc_fs.h?v=3.10
     *  3.9 version: http://lxr.free-electrons.com/source/include/linux/proc_fs.h?v=3.9
     *
     *  You can find greater explanation of using full featured /proc functions in the book Linux Device Drivers 4,
     *  or, if you want shorter solution, check this link (https://github.com/jesstess/ldd4/blob/master/scull/main.c)
     *  where you can see how the struct file_operations has been used. You do not have to setup to all fields of the struct.
     *
     *  but the function  remove_proc_remove, you can do nothing for it, becase there are tow function for it
     *  static inline void proc_remove(struct proc_dir_entry *de) {}
     *  #define remove_proc_entry(name, parent) do {} while (0)
     */

    /// create a file named "pid" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_pid = create_proc_entry("pid", PERMISSION, dir);

    if(proc_pid == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/pid\n");
        ret = FAIL;

        goto create_pid_failed;
	}
	proc_pid->write_proc = proc_write_pid;  /// write only
	//proc_pid->owner = THIS_MODULE;

    dbginfo("CREATE_PROC_ENTRY ");

#elif defined PROC_CREATE

    proc_pid = proc_create("pid", PERMISSION, dir, &proc_pid_fops);

    if(proc_pid == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/pid\n");
        ret = FAIL;

        goto create_pid_failed;
	}
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/pid success...\n");

    /// create a file named "virtualAddr" in direntory
#ifdef CREATE_PROC_ENTRY

	proc_va = create_proc_entry("virtualAddr", PERMISSION, dir);
	if(proc_va == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/virtualAddr\n");
        ret = FAIL;

        goto create_va_failed;
	}
	proc_va->read_proc = proc_read_va;         // can read
	proc_va->write_proc = proc_write_va;       // can write
	//proc_va->owner = THIS_MODULE;

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_va = proc_create("virtualAddr", PERMISSION, dir, &proc_va_fops);

    if(proc_va == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/virtualAddr\n");
        ret = FAIL;

        goto create_va_failed;
    }
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/virtualAddr success...\n");


   ///  create a file named "ctl" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_ctl = create_proc_entry("ctl", PERMISSION, dir);
	if(proc_ctl == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/ctl\n");
        ret = FAIL;

        goto create_ctl_failed;

	}

    proc_ctl->write_proc = proc_write_ctl;              // write only
	//proc_ctl->owner = THIS_MODULE;

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_ctl = proc_create("ctl", PERMISSION, dir, &proc_ctl_fops);

    if(proc_ctl == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/ctl\n");
        ret = FAIL;

	    goto create_ctl_failed;
    }
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/ctl success...\n");

    ///  create a file named "signal" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_signal = create_proc_entry("signal", PERMISSION, dir);

    if(proc_signal == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/signal\n");
        ret = FAIL;

        goto create_signal_failed;
	}
	proc_signal->read_proc = proc_read_signal;          //  can read
	proc_signal->write_proc = proc_write_signal;        //  can write
	//proc_signal->owner = THIS_MODULE;

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_signal = proc_create("signal", PERMISSION, dir, &proc_signal_fops);

    if(proc_signal == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/signal\n");
        ret = FAIL;

        goto create_signal_failed;
	}
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/signal success\n");


    ///  create a file named "physicalAddr" in direntory
#ifdef CREATE_PROC_ENTRY

	proc_pa = create_proc_entry("physicalAddr", PERMISSION, dir);
	if(proc_pa == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/physicalAddr\n");
        ret = FAIL;

        goto create_pa_failed;
	}

	proc_pa->read_proc = proc_read_pa;                  //  can read
	proc_pa->write_proc = proc_write_pa;                //  can write

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_pa = proc_create("physicalAddr", PERMISSION, dir, &proc_pa_fops);

    if(proc_pa == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/physicalAddr\n");
        ret = FAIL;

        goto create_pa_failed;
    }
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/physicalAddr success\n");


    ///  create a file named "kFuncName" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_kFuncName = create_proc_entry("kFuncName", PERMISSION, dir);

    if(proc_kFuncName == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/kFuncName\n");
        ret = FAIL;

        goto create_kFuncName_failed;

    }
	proc_kFuncName->write_proc = proc_write_kFuncName;  // write only

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_kFuncName = proc_create("kFuncName", PERMISSION, dir, &proc_kFuncName_fops);

    if(proc_kFuncName == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/kFuncName\n");
        ret = FAIL;

        goto create_kFuncName_failed;
    }
    dbginfo("PROC_CREATE ");
#endif
	printk("Create /proc/memoryEngine/kFuncName success\n");

    ///  create a file named "taskInfo" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_taskInfo = create_proc_entry("taskInfo", PERMISSION, dir);

    if(proc_taskInfo == NULL)
	{
	    dbginfo("Can't create /proc/memoryEngine/taskInfo\n");
        ret = FAIL;

        goto create_taskInfo_failed;
	}
	proc_taskInfo->read_proc = proc_read_taskInfo;      // read only

    dbginfo("CREATE_PROC_ENTRY ");
#elif defined PROC_CREATE

    proc_taskInfo = proc_create("taskInfo", PERMISSION, dir, &proc_taskInfo_fops);

    if(proc_taskInfo == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/taskInfo\n");
        ret = FAIL;

        goto create_taskInfo_failed;
	}
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/taskInfo success\n");

    ///  create a file named "memVal" in direntory
#ifdef CREATE_PROC_ENTRY

    proc_val = create_proc_entry("memVal", PERMISSION, dir);

    if(proc_val == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/memVal\n");
		ret = FAIL;

        goto create_val_failed;
	}

    proc_val->write_proc = proc_write_memVal;           // can write
	proc_val->read_proc = proc_read_memVal;             // can read

    dbginfo("CREATE_PROC_ENTRY ");

#elif defined PROC_CREATE

    proc_val = proc_create("memVal", PERMISSION, dir, &proc_val_fops);

    if(proc_val == NULL)
	{
		dbginfo("Can't create /proc/memoryEngine/memVal\n");
        ret = FAIL;

        goto create_val_failed;
    }
    dbginfo("PROC_CREATE ");
#endif
    printk("Create /proc/memoryEngine/memVal success\n");

    ret = register_jprobe(&jprobe1);
	if (ret < 0)
	{
		printk("register_jprobe jprobe1 failed, returned %d\n", ret);
        ret = FAIL;

        goto register_jprobe_failed;
	}
	printk("Planted jprobe at force_sig_info: %p\n", jprobe1.kp.addr);

	dbginfo("Memory engine module init\n");

    return OK;


//  remove the proc files
register_jprobe_failed  :
    remove_proc_entry("memVal", dir);

create_val_failed       :
    remove_proc_entry("kFuncName", dir);

create_kFuncName_failed :
    remove_proc_entry("taskInfo", dir);

create_taskInfo_failed  :
    remove_proc_entry("physicalAddr", dir);

create_pa_failed        :
    remove_proc_entry("signal", dir);

create_signal_failed    :
    remove_proc_entry("ctl", dir);

create_ctl_failed       :
    remove_proc_entry("virtualAddr", dir);

create_va_failed        :
    remove_proc_entry("pid", dir);

create_pid_failed       :
    remove_proc_entry("memoryEngine", NULL);

    return ret;

}

/// modify by gatieme for system porting NeoKylin-linux-3.14/16 @ 2016--03-28 20:08
// invalid storage class for function ‘jforce_sig_info’
static int jforce_sig_info(int sig, struct siginfo *info, struct task_struct *t)
{
    printk("MemSysFI: kernel is sending signal %d to process pid: %d, comm: %s\n", sig, t->pid, t->comm);
/*
    if (f_inject == 'N')
    {
        jprobe_return();
        return 0;
    }

    down_interruptible(&sem);
    if ( addone(addone(inj_info.rear))==inj_info.front )
    {
		*/
        /*error:队列满*/
       /*
	   sprintf(inj_info.inj_log[inj_info.rear].msg,"caution : buf is full, messages have been dropped\n");
    }
    else
    {
        inj_info.rear = addone(inj_info.rear);
        sprintf(inj_info.inj_log[inj_info.rear].msg,"warning : kernel is sending signal %d to process pid: %d, comm: %s\n",sig,current->pid,current->comm);
        //inj_info->inj_log[inj_info->rear ] = x ;
    }
    up(&sem);
	*/
    /*
    	if(message!=NULL)
    	{
    		for(i=0;i<256;i++)
    			message[i]='\0';

    		sprintf(message,"warning : kernel is sending signal %d to process pid: %d, comm: %s\n\0",sig,current->pid,current->comm);
    	}
    	else
    		return -1;


    	struct bufferList *buflist;
    	buflist = (struct bufferList *)kmalloc(sizeof(struct bufferList),GFP_KERNEL);
    	buflist->pNext = NULL;
    	buflist->buffer = message;

    	down_interruptible(&sem);
    	if(Head==NULL)
    	{
    		Head = buflist;
    		Tail = Head;
    	}
    	else
    	{
    	  Tail->pNext = buflist;
    	  Tail = buflist;
    	}
    	flag = 1;
    	up(&sem);
    	*/

//	wake_up_interruptible(&wq2);

    jprobe_return();
    return 0;
}

/*
*  uninit memory fault injection module
*/
static void __exit exitME(void)
{
	remove_proc_entry("pid", dir);
    printk("Remove /proc/memoryEngine/pid success\n");

    remove_proc_entry("virtualAddr", dir);
    printk("Remove /proc/memoryEngine/virtualAddr success\n");

	remove_proc_entry("ctl", dir);
    printk("Remove /proc/memoryEngine/ctl success\n");

    remove_proc_entry("signal", dir);
    printk("Remove /proc/memoryEngine/signal success\n");

    remove_proc_entry("physicalAddr", dir);
    printk("Remove /proc/memoryEngine/physicalAddr success\n");

    remove_proc_entry("kFuncName", dir);
    printk("Remove /proc/memoryEngine/kFuncName success\n");

	remove_proc_entry("taskInfo", dir);
    printk("Remove /proc/memoryEngine/taskinfo success\n");

    remove_proc_entry("memVal", dir);
    printk("Remove /proc/memoryEngine/memVal success\n");

    remove_proc_entry("memoryEngine", NULL);
    printk("Remove /proc/memoryEngine success\n");

    unregister_jprobe(&jprobe1);
	printk("jprobe at %p unregistered.\n",	jprobe1.kp.addr);
	dbginfo("Memory engine module exit\n");
}

module_init(initME);
module_exit(exitME);

MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("Gatieme @ HIT CS HDMC team");

MODULE_DESCRIPTION("Memory Engine Module.");

