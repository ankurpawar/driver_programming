#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/string.h>
#include<linux/uaccess.h>

#define MAGIC_NUM "196"
#define MAX_LEN 32

static int id_len;

ssize_t misc_write(struct file *filep, const char __user *ubuf, size_t count,
		   loff_t *f_pos)
{
	int ret = 0;
	char buff[MAX_LEN] = { 0 };

	ret = simple_write_to_buffer(buff, MAX_LEN, f_pos, ubuf, count);
	if (ret < 0)
		return ret;
	if (strcmp(buff, MAGIC_NUM))
		ret = -EINVAL;
	return ret;
}

ssize_t misc_read(struct file *filep, char __user *buf, size_t count,
		  loff_t *fpos)
{
	return simple_read_from_buffer(buf, count, fpos, MAGIC_NUM, id_len);
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice my_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_drv",
	.fops = &my_fops,
};

static int __init misc_init(void)
{
	int retval = 0;
	id_len = strlen(MAGIC_NUM);
	pr_debug("hello world module from a misc\n");

	retval = misc_register(&my_dev);
	if (retval)
		pr_debug("error in misc registration\n");
	return retval;
}

module_init(misc_init);

static void __exit misc_exit(void)
{
	int retval = 0;
	pr_debug("good bye world from a misc\n");
	retval = misc_deregister(&my_dev);
	if (retval)
		pr_debug("error in deregister\n");
}

module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("a_misc");
MODULE_DESCRIPTION("simple misc driver");
MODULE_AUTHOR("Ankur");
