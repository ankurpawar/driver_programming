#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/debugfs.h>
#include<linux/uaccess.h>

#define ID "Hello world"
#define MAX_LEN 32

static struct dentry *debug_dir; 
static struct dentry *debug_file; 
static int i;

ssize_t dev_write(struct file *filep, const char __user *ubuf, size_t count,
		   loff_t *f_pos)
{
	int ret = strlen(ID);
	char buff[MAX_LEN] = { 0 };
	if (count > MAX_LEN)
		count = MAX_LEN;
	if (copy_from_user(buff, ubuf, count))
		return -EFAULT;

	if (strcmp(buff, ID))
		ret = -EINVAL;
	return ret;
}

ssize_t dev_read(struct file *filep, char __user *buf, size_t count,
		  loff_t *fpos)
{
	int ret = 0;
	char *id = ID;
	int len = strlen(ID);
	if (*fpos >= len)
		goto RET;

	if (copy_to_user(buf, id, len))
		return -EFAULT;
	ret = len;
	*fpos = len;
RET:
	return ret;
}

int dev_open(struct inode *inodep, struct file *filep)
{
	return 0;
}

int dev_close(struct inode *inodep, struct file *filep)
{
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_close,
	.read = dev_read,
	.write = dev_write,
};

static int __init dev_init(void)
{
	int retval = 0;
	pr_debug("hello world module from a rookie\n");
	debug_dir = debugfs_create_dir("a_rookie", NULL);
	if (debug_dir == NULL)
		return -EFAULT;
	debug_file = debugfs_create_file("filetxt", 0644, 
			debug_dir, &i, &my_fops);
	return retval;
}

module_init(dev_init);

static void __exit dev_exit(void)
{
	debugfs_remove_recursive(debug_dir);
	pr_debug("good bye world from a misc\n");
}

module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("simple_debugfs");
MODULE_DESCRIPTION("simple dbugfs driver");
MODULE_AUTHOR("Ankur");
