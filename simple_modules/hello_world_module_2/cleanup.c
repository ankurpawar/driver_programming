#include"header.h"

static void __exit cleanup(void)
{
	printk(KERN_INFO "good bye world\n");
}

module_exit(cleanup); 
