#include "proc_ops.h"
#include "header.h"

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

struct seq_operations scull_seq_ops = {
	.start = scull_seq_start,
	.next = scull_seq_next,
	.show = scull_seq_show,
	.stop = scull_seq_stop
};

void *scull_seq_start(struct seq_file *filep, loff_t *pos)
{
	if(*pos >= num_dev)
		return NULL;
	/*return the next device structure in array*/
	return (sculldev + *pos);

}

void *scull_seq_next(struct seq_file *filep, void *v, loff_t *pos)
{
	(*pos)++;
	if(*pos >= num_dev)
		return NULL;
	return (sculldev + *pos);
}

void scull_seq_stop(struct seq_file *filep, void *v)
{
	/*scull implementation has no cleanup work to do,so it is empty*/
}

int scull_seq_show(struct seq_file *filep, void *v)
{
	struct ScullDev *sdev = (struct ScullDev *)v;
	if(down_interruptible(&sdev->sem))
		return -ERESTARTSYS;
	seq_printf(filep,"Dev = %d,qset_size = %d,quant_size = %d, data_size = %d\n",
		(int)(sdev - sculldev), sdev->qset_size , sdev->quantum_size , sdev->data_size);
	up(&sdev->sem);
	return 0;
}
