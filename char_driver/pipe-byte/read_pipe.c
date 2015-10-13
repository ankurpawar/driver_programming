#include"header.h"
#include"declarations.h"

ssize_t read_pipe(struct file *filep,char __user *buf,size_t count,loff_t *off)
{
	struct ScullPipe *lscullpipe;
	int ret;
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	lscullpipe = filep->private_data;
	if (!lscullpipe) {
		printk(KERN_ERR "scullpipe not found\n");
		goto ERR;
	}
	#ifdef DEBUG
        printk(KERN_INFO "before wait\n");
        #endif	
	wait_event_interruptible(lscullpipe->outq,lscullpipe->buffer_size > 0);

	if (down_interruptible(&lscullpipe->sem))
		return -ERESTARTSYS;

	if (lscullpipe->rp >= lscullpipe->end)
		lscullpipe->rp = lscullpipe->buffer;
	ret = copy_to_user(buf,lscullpipe->rp,1);
	if (ret) {
		printk(KERN_ERR "partial read,ret = %d\n",ret);
	}

	lscullpipe->rp++;
	lscullpipe->buffer_size--;
	up(&lscullpipe->sem);
	wake_up_interruptible(&lscullpipe->inq);
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return 1;
ERR:
	return -1;
}
