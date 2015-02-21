#include"header.h"
#include"declarations.h"

ssize_t write_pipe(struct file *filep,const char __user *buf,size_t count,loff_t *off)
{
	int ret;
	struct ScullPipe *lscullpipe;
	char *pipe;
	int nobtw = 0;
	int wbtw = 0; /*wrapped bytes to write*/
	int num_bytes;
	char wrap = 0;
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
	/*if bytes to write exceeds pipe size*/
	if ((lscullpipe->buffer_size+count) > max_pipe_size)
		nobtw = max_pipe_size - lscullpipe->buffer_size;
	else
		nobtw = count;

	#ifdef DEBUG
        printk(KERN_INFO "number of bytes to write = %d\n",nobtw);
	#endif
	
	/*check wrap around*/
	if ((lscullpipe->wp+nobtw) > lscullpipe->end) {
		num_bytes = lscullpipe->end - lscullpipe->wp;
		wbtw = nobtw - num_bytes;
		nobtw = num_bytes;
		wrap = 1;
		#ifdef DEBUG
		printk(KERN_INFO "nobtw = %d,wbtw = %d\n",nobtw,wbtw);
		#endif
	}

	if (lscullpipe->wp >= lscullpipe->end)
		lscullpipe->wp = lscullpipe->buffer;
	ret = copy_from_user(lscullpipe->wp,buf,nobtw);
	if (ret) {
		printk(KERN_ERR "copy_from_user unable to write %d bytes\n",ret);
		goto ERR;
	}

	lscullpipe->wp += nobtw;
	lscullpipe->buffer_size += nobtw;

	if (wrap) {
		if (lscullpipe->wp >= lscullpipe->end)
			lscullpipe->wp = lscullpipe->buffer;
		ret = copy_from_user(lscullpipe->wp,buf,wbtw);
		if (ret) {
			printk(KERN_ERR "copy_from_user unable to write %d bytes\n",ret);
			goto ERR;
		}
		lscullpipe->wp += wbtw;
		lscullpipe->buffer_size += wbtw;
	}
	up(&lscullpipe->sem);
	wake_up_interruptible(&lscullpipe->outq);
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	ret = nobtw + wbtw;
	return ret;
ERR:
	up(&lscullpipe->sem);
	return -1;
}
