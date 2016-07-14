/******************************************************************************
 * privcmd.h
 * 
 * Interface to /proc/xen/privcmd.
 * 
 * Copyright (c) 2003-2005, K A Fraser
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __LINUX_PUBLIC_PRIVCMD_H__
#define __LINUX_PUBLIC_PRIVCMD_H__

#include <linux/types.h>

#ifdef __KERNEL__
#include <xen/interface/hvm/hvm_op.h>
#include <xen/interface/memory.h>
#else
#include <xen/hvm/hvm_op.h>
#include <xen/memory.h>
#endif

#ifndef __user
#define __user
#endif

typedef struct privcmd_hypercall
{
	__u64 op;
	__u64 arg[5];
} privcmd_hypercall_t;

typedef struct privcmd_mmap_entry {
	__u64 va;
	__u64 mfn;
	__u64 npages;
} privcmd_mmap_entry_t; 

typedef struct privcmd_mmap {
	int num;
	domid_t dom; /* target domain */
	privcmd_mmap_entry_t __user *entry;
} privcmd_mmap_t; 

typedef struct privcmd_mmapbatch {
	int num;     /* number of pages to populate */
	domid_t dom; /* target domain */
	__u64 addr;  /* virtual address */
	xen_pfn_t __user *arr; /* array of mfns - top nibble set on err */
} privcmd_mmapbatch_t; 

typedef struct privcmd_mmapbatch_v2 {
	unsigned int num; /* number of pages to populate */
	domid_t dom;      /* target domain */
	__u64 addr;       /* virtual address */
	const xen_pfn_t __user *arr; /* array of mfns */
	int __user *err;  /* array of error codes */
} privcmd_mmapbatch_v2_t;

typedef struct privcmd_restrict_domid {
       domid_t domid;
} privcmd_restrict_domid_t;

typedef struct privcmd_memop {
        unsigned cmd;
        union {
                xen_memory_reservation_t reservation;
                xen_memory_exchange_t exchange;
                domid_t domid;
                xen_add_to_physmap_t add_to_physmap;
                xen_foreign_memory_map_t foreign_memory_map;
                xen_machphys_mfn_list_t machphys_mfn_list;
                xen_machphys_mapping_t machphys_mapping;
                xen_memory_map_t memory_map;
        } u;
} privcmd_memop_t;

typedef struct privcmd_hvmop {
        unsigned cmd;
        union {
                xen_hvm_param_t param;
                xen_hvm_set_pci_intx_level_t set_pci_intx_level;
                xen_hvm_set_isa_irq_level_t set_isa_irq_level;
                xen_hvm_set_pci_link_route_t set_pci_link_route;
                xen_hvm_modified_memory_t modified_memory;
                xen_hvm_set_mem_type_t set_mem_type;
                xen_hvm_track_dirty_vram_t track_dirty_vram;
        } u;
} privcmd_hvmop_t;

/*
 * @cmd: IOCTL_PRIVCMD_HYPERCALL
 * @arg: &privcmd_hypercall_t
 * Return: Value returned from execution of the specified hypercall.
 */
#define IOCTL_PRIVCMD_HYPERCALL					\
	_IOC(_IOC_NONE, 'P', 0, sizeof(privcmd_hypercall_t))
#define IOCTL_PRIVCMD_MMAP					\
	_IOC(_IOC_NONE, 'P', 2, sizeof(privcmd_mmap_t))
#define IOCTL_PRIVCMD_MMAPBATCH					\
	_IOC(_IOC_NONE, 'P', 3, sizeof(privcmd_mmapbatch_t))
#define IOCTL_PRIVCMD_MMAPBATCH_V2				\
	_IOC(_IOC_NONE, 'P', 4, sizeof(privcmd_mmapbatch_v2_t))
#define IOCTL_PRIVCMD_RESTRICT_DOMID                           \
       _IOC(_IOC_NONE, 'P', 100, sizeof(privcmd_restrict_domid_t))
#define IOCTL_PRIVCMD_DOMCTL                           \
       _IOC(_IOC_NONE, 'P', 101, sizeof(xen_domctl_t))
#define IOCTL_PRIVCMD_HVMOP                                    \
       _IOC(_IOC_NONE, 'P', 102, sizeof(privcmd_hvmop_t))
#define IOCTL_PRIVCMD_SHUTDOWN                                 \
       _IOC(_IOC_NONE, 'P', 103, sizeof(sched_remote_shutdown_t))
#define IOCTL_PRIVCMD_MEMOP                                    \
       _IOC(_IOC_NONE, 'P', 104, sizeof(privcmd_memop_t))

#define ARCH_IOCTL(d, r, a) ioctl(d, r, a)

#endif /* __LINUX_PUBLIC_PRIVCMD_H__ */
