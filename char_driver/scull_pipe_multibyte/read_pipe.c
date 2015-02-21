#include"header.h"
#include"declarations.h"

ssize_t read_pipe(struct file *filep,char __user *buf,size_t count,loff_t *off)
{
	struct ScullPipe *lscullpipe;
	int ret;
	int nobtr = 0;
	int wbtr = 0;
	int wrap = 0;
	int num_bytes = 0;
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

	/*if count exceeds pipe size*/
	if (count > lscullpipe->buffer_size)
		nobtr = lscullpipe->buffer_size;
	else
		nobtr = count;

	if ((lscullpipe->rp + nobtr) > lscullpipe->end) {
		num_bytes = lscullpipe->end - lscullpipe->rp;
		wrap = 1;
		wbtr = nobtr - num_bytes;
		nobtr = num_bytes;
	}
	
	if (lscullpipe->rp >= lscullpipe->end)
		lscullpipe->rp = lscullpipe->buffer;
	ret = copy_to_user(buf,lscullpipe->rp,nobtr);
	if (ret) {
		printk(KERN_ERR "partial read,ret = %d\n",ret);
		goto ERR;
	}

	lscullpipe->rp += nobtr;
	lscullpipe->buffer_size -= nobtr;
	
	if (wrap) {
		if (lscullpipe->rp >= lscullpipe->end)
			lscullpipe->rp = lscullpipe->buffer;
		ret = copy_to_user(buf,lscullpipe->rp,nobtr);
		if (ret) {
			printk(KERN_ERR "partial read,ret = %d\n",ret);
			goto ERR;
		}
		lscullpipe->wp += wbtr;
		lscullpipe->buffer_size -= wbtr;
        }

	up(&lscullpipe->sem);
	wake_up_interruptible(&lscullpipe->inq);
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	ret = nobtr + wbtr;
	return ret;
ERR:
	up(&lscullpipe->sem);
	return -1;
}
