#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");

static int __init initialization(void)
{
	printk(KERN_INFO "hello world module\n");
	return 0;
}
module_init(initialization);

static void __exit cleanup(void)
{
	printk(KERN_INFO "good bye world\n");
}
module_exit(cleanup); 
