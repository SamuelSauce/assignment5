#include <linux/cdev.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cred.h>

MODULE_AUTHOR("");
MODULE_LICENSE("GPL");

static int __init shady_init_module(void) {
  printk(KERN_INFO "shady module loaded\n");
  return 0;
}


static void __exit shady_exit_module(void) {
  printk(KERN_INFO "shady module unloaded\n");
}

module_init(shady_init_module);
module_exit(shady_exit_module);
