#include"header.h"
#include"declarations.h"

struct file_operations scull_proc_ops =
{
	.owner = THIS_MODULE,
	.open = scull_proc_open,
        .read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

int scull_proc_open(struct inode *node,struct file *file)
{
	return seq_open(file, &scull_seq_ops);
}
