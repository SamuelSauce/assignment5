#include <linux/cdev.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <linux/unistd.h>
MODULE_AUTHOR("");
MODULE_LICENSE("GPL");

const unsigned long system_call_table_address =  0xffffffff81801400;

void set_addr_rw(unsigned long addr){
	unsigned int level;
	pte_t *pte = lookup_address(addr, &level);
	if(pte->pte &~ _PAGE_RW) pte->pte |= _PAGE_RW;
}
asmlinkage int (*old_open) (const char*, int, int);

asmlinkage int my_open(const char* file, int flags, int mode)
{
	printk("Mark is opening a file.\n");
	return old_open(file, flags, mode);
}

static int __init shady_init_module(void) {
	int i;
	void** sct = (void**)kallsyms_lookup_name("sys_call_table");
	unsigned long opn_addr = kallsyms_lookup_name("sys_open");
	set_addr_rw((unsigned long) system_call_table_address);
	old_open = (void *)kallsyms_lookup_name("sys_open");
	for(i = 0; i < 20; i++)
	{
		if((unsigned long)sct[i] == opn_addr){
			sct[i] = my_open;
			
		}
	}
  	printk(KERN_INFO "shady module loaded\n");
  	return 0;
}


static void __exit shady_exit_module(void) {
	int i;
	void** sct = (void**)kallsyms_lookup_name("sys_call_table");
	unsigned long opn_addr = kallsyms_lookup_name("sys_open");
	for(i = 0; i < 20; i++)
	{
		if((unsigned long)sct[i] == opn_addr)
		{
			sct[i] = old_open;
		}
	}
	printk(KERN_INFO "shady module unloaded\n");
}

module_init(shady_init_module);
module_exit(shady_exit_module);
