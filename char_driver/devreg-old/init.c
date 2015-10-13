#include"header.h"

int major_num;
struct file_operations fops=
{};

void init_major(void)
{
	major_num = 0;
}

static int __init initialization(void)
{
	init_major();
	printk(KERN_INFO "hello world module\n");
	major_num = register_chrdev(major_num,DEV_NAME,&fops);
	if(major_num < 0) {
		printk(KERN_ERR "registration failure\n");
		goto OUT;
	}
	printk(KERN_INFO"major num allocated=%d\n",major_num);
	return 0;
OUT:	return -1; 
}

module_init(initialization);
