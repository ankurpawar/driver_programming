#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

static struct miscdevice my_dev;

static struct fops my_fops = {
	.owner = THIS_MODULE;
	.open = misc_open;
	.close = misc_close;
	.read = misc_read;
	.write = misc_write;
};

static int __init misc_init(void)
{
	int retval = 0;
	my_dev.minor = MISC_DEVICE_MINOR;
	my_dev.name = "my_misc";
	my_dev.fops = &my_fops;
	printk(KERN_INFO "hello world module from a misc\n");
	return 0;
}
module_init(misc_init);

static void __exit misc_exit(void)
{
        printk(KERN_INFO "good bye world from a misc\n");
}
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("a_misc");
MODULE_DESCRIPTION("simple misc driver");
MODULE_AUTHOR("Ankur");
