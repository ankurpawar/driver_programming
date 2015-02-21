#include"header.h"
#include"declarations.h"

int open_pipe(struct inode *inodep,struct file *filep)
{
	struct ScullPipe *lscullpipe;
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
	
	if (down_interruptible(&lscullpipe->sem))
		return -ERESTARTSYS;
	filep->private_data = lscullpipe;
	if ((filep->f_flags & O_ACCMODE) == O_WRONLY) {
		/*allocate pipe buffer if not allocated*/
		if(!lscullpipe->buffer) {
			lscullpipe->buffer = create_pipe(max_pipe_size);
			if (!lscullpipe->buffer) {
				printk(KERN_ERR "error in creating pipe");
				goto SEM_UP;
			}
			lscullpipe->rp = lscullpipe->buffer;
			lscullpipe->wp = lscullpipe->buffer;
			lscullpipe->end = lscullpipe->buffer + max_pipe_size;
		}
		lscullpipe->num_writer++;
		#ifdef DEBUG
		printk(KERN_INFO "number of writers %d\n",lscullpipe->num_writer);
		#endif
	}

	if ((filep->f_flags & O_ACCMODE) == O_RDONLY) {
		lscullpipe->num_reader++;
		#ifdef DEBUG
		printk(KERN_INFO "number of readers %d\n",lscullpipe->num_reader);
		#endif
	}

	up(&lscullpipe->sem);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
SEM_UP:
	up(&lscullpipe->sem);
ERR:
	return -1;
}
