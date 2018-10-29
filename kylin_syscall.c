#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/syscall.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>
#include <generated/compile.h>
#include <linux/uts.h>
#include <generated/utsrelease.h>
#include <asm/cacheflush.h>
#include "syscall.h"

/* Notes
 * 1. On x86 platform, we have defined the section including sys_call_table to RO, so We 
 * cann't excute the replace operation, like sys_call_table[x] = ?, directly without any
 * changes. Otherwise we may encounter a Oops.
 *
 * 2. set_memory_rw could be used to change a page's read/write property, but it doesn't
 * success always, seeing details from the bellow link:
 * https://stackoverflow.com/questions/2103315/linux-kernel-system-call-hooking-example
 *
 * 3. For most case, we could modify the pte associated with a address to writable using lookup_address 
 * 4. we can't use snprintf to directly copy data to user space buffer, otherwise you may encounter
 * another Oops, copy_to_user is the best and safe choice.
 */

int (*make_memory_rw)(unsigned long addr, int numpages);
void set_addr_rw(unsigned long addr) {
	unsigned int level;
	pte_t *pte = lookup_address(addr, &level);
	pte->pte |= _PAGE_RW;	
}
static void **kylinversion;
unsigned long org_entry = 0;

asmlinkage long sys_kylinversion(char __user *buf, size_t len)
{
	void *kbuf = (void *)kzalloc(len, GFP_KERNEL);
	printk("hello kylin, i am in system call len=%u\n",len);
	snprintf(kbuf, len, "%s %s %s %s%s%s%s%s %s%s%s %s\n",UTS_SYSNAME,"version",UTS_RELEASE,"(",LINUX_COMPILE_BY,"@",LINUX_COMPILE_HOST,")","(",LINUX_COMPILER,")",UTS_VERSION);	
	copy_to_user(buf, kbuf, len);
	kfree(kbuf);
	return 0;
}

int __init sys_kylinversion_init(void)
{
	kylinversion = (void *)(kallsyms_lookup_name("sys_call_table"));
#ifndef	CONFIG_DEBUG_RODATA
	make_memory_rw = (void *)(kallsyms_lookup_name("set_memory_rw"));
	/* must align to page size */
	make_memory_rw(PAGE_ALIGN((long unsigned int)kylinversion) - PAGE_SIZE, 1);
#else
	set_addr_rw((unsigned long)kylinversion);
#endif
	org_entry = kylinversion[_NR_kylinversion];
	kylinversion[_NR_kylinversion] = sys_kylinversion;
	printk("system call sys_kylinversion loaded success\n");
	printk("version: %s %s %s\n",utsname()->sysname,utsname()->release,utsname()->version);
	printk("%s %s %s %s%s%s%s%s %s%s%s %s\n",UTS_SYSNAME,"version",UTS_RELEASE,"(",LINUX_COMPILE_BY,"@",LINUX_COMPILE_HOST,")","(",LINUX_COMPILER,")",UTS_VERSION);
	return 0;
}

void __exit sys_kylinversion_exit(void)
{
	if (org_entry)
		kylinversion[_NR_kylinversion] = org_entry;
	printk("system call sys_helloworld unloaded\n");
}

module_init(sys_kylinversion_init);
module_exit(sys_kylinversion_exit);
MODULE_DESCRIPTION("The demo of add a system call");
MODULE_LICENSE("GPL");

