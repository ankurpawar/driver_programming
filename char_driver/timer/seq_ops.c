#include"header.h"
#include"declarations.h"

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
	struct jit_data *ljit;
	char *buf = NULL;
	int ret;
	unsigned long j1,j2;
	#ifdef DEBUG
	printk(KERN_INFO "START : %s \n",__func__);
	#endif
	ret = seq_get_buf(filep,&buf);
	#ifdef DEBUG
	printk(KERN_INFO "ret = %d , buf = %p\n",ret,buf);
	#endif
	if (buf == NULL)
		return -1;

	ljit = kmalloc(sizeof(struct jit_data), GFP_KERNEL);
	if (!ljit)
		return -ENOMEM;

	init_timer(&ljit->timer);
	ljit->timer.data = (unsigned long)ljit;
	ljit->buf = buf;
	ljit->timer.function = jit_func;
	ljit->timer.expires = 1;
	ljit->flag = 0;
	ljit->r_time = sdev->r_time;
	ljit->w_time = sdev->w_time;
	init_waitqueue_head(&ljit->waitq);

	if(down_interruptible(&sdev->sem))
		return -ERESTARTSYS;
	j1 = jiffies;
	/*seq_printf(filep,"Dev = %d,qset_size = %d,quant_size = %d, data_size = %d\n",
		(int)(sdev - sculldev), sdev->qset_size , sdev->quantum_size , sdev->data_size);*/
	ret = sprintf(buf,"Dev = %d,qset_size = %d,quant_size = %d, data_size = %d\n",
		(int)(sdev - sculldev), sdev->qset_size , sdev->quantum_size , sdev->data_size);
	seq_commit(filep,ret);
	ret = seq_get_buf(filep,&buf);
	ljit->count = 0;
	ljit->buf = buf;

	add_timer(&ljit->timer);
	up(&sdev->sem);
	wait_event_interruptible(ljit->waitq,ljit->flag);
	seq_commit(filep,ljit->count);
	j2 = jiffies;
	#ifdef DEBUG
	printk(KERN_INFO "jiffies diff = %li\n",(long)j2-(long)j1);
	#endif
	
	kfree(ljit);
	#ifdef DEBUG
	printk(KERN_INFO "END : %s \n",__func__);
	#endif
	return 0;
}
