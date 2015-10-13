#include"header.h"
#include"declarations.h"

ssize_t write_pipe(struct file *filep,const char __user *buf,size_t count,loff_t *off)
{
	int ret;
	struct ScullPipe *lscullpipe;
	char *pipe;
	int num_bytes; 
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	lscullpipe = filep->private_data;
	if (!lscullpipe) {
		printk(KERN_ERR "no pipe present to write\n");
		goto ERR;
	}

	pipe = lscullpipe->buffer;
	num_bytes = lscullpipe->buffer_size;
	#ifdef DEBUG
        printk(KERN_INFO "before wait\n");
        #endif
	wait_event_interruptible(lscullpipe->inq,num_bytes < max_pipe_size);
	
	if (down_interruptible(&lscullpipe->sem))
		return -ERESTARTSYS;

	if (lscullpipe->wp >= lscullpipe->end)
		lscullpipe->wp = lscullpipe->buffer;
	ret = copy_from_user(lscullpipe->wp,buf,1);
	if (ret) {
		printk(KERN_ERR "partial write ret = %d\n",ret);
	}

	lscullpipe->wp++;
	lscullpipe->buffer_size++;
	up(&lscullpipe->sem);
	wake_up_interruptible(&lscullpipe->outq);
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return 1;
ERR:
	up(&lscullpipe->sem);
	return -1;
}
