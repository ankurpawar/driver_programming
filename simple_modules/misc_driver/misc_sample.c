#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>

static struct miscdevice my_dev;


ssize_t misc_write(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	return 0;
}

ssize_t misc_read(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	return 0;
}

int misc_open(struct inode *inodep, struct file *filep)
{
	return 0;
}

int misc_close(struct inode *inodep,struct file *filep)
{
	return 0;
}

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_close,
	.read = misc_read,
	.write = misc_write,
};

static int __init misc_init(void)
{
	int retval = 0;
	my_dev.minor = MISC_DYNAMIC_MINOR;
	my_dev.name = "my_misc";
	my_dev.fops = &my_fops;
	printk(KERN_INFO "hello world module from a misc\n");
	
	retval = misc_register(&my_dev);
	if (retval)
		printk("error in misc registration\n");
	return retval;
}
module_init(misc_init);

static void __exit misc_exit(void)
{
	int retval = 0;
        printk(KERN_INFO "good bye world from a misc\n");
	retval = misc_deregister(&my_dev);
	if (retval)
		printk("error in deregister\n");
}
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("a_misc");
MODULE_DESCRIPTION("simple misc driver");
MODULE_AUTHOR("Ankur");
