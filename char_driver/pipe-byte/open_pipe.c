#include"header.h"
#include"declarations.h"

int open_pipe(struct inode *inodep,struct file *filep)
{
	struct ScullPipe *lscullpipe;
	int ret;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	lscullpipe = (struct ScullPipe*)&scullpipe[0];
	if(!scullpipe) {
		printk(KERN_ERR "sculldev not allocated");
		goto ERR;
	}
	
	lscullpipe = container_of(inodep->i_cdev,struct ScullPipe,c_dev);
	if (!lscullpipe) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = lscullpipe;
	if ((filep->f_flags & O_ACCMODE) == O_WRONLY) {
		#ifdef DEBUG
		printk(KERN_INFO "device write mode\n");
		#endif
		if (down_interruptible(&lscullpipe->sem))
			return -ERESTARTSYS;

		ret = trim_pipe(lscullpipe);
		if (ret < 0) {
			#ifdef DEBUG
			printk(KERN_ERR "device trim failed!\n");
			#endif
			goto SEM_UP;
		}
		lscullpipe->buffer = create_pipe(max_pipe_size);
		if (!lscullpipe->buffer) {
			printk(KERN_ERR "erorr in creating pipe");
			goto SEM_UP;
		}
		lscullpipe->rp = lscullpipe->buffer;
		lscullpipe->wp = lscullpipe->buffer;
		lscullpipe->end = lscullpipe->buffer + (max_pipe_size - 1);
		up(&lscullpipe->sem);
	}
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
SEM_UP:
	up(&lscullpipe->sem);
ERR:
	return -1;
}
