#include"header.h"

static int __init initialization(void)
{
	printk(KERN_INFO "hello world module\n");
	return 0;
}

module_init(initialization);
