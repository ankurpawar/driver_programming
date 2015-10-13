#include"header.h"
#include"declarations.h"

int close_pipe(struct inode *inodep,struct file *filep)
{
	struct ScullPipe *lscullpipe; 
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	
	lscullpipe = filep->private_data;
	if (!lscullpipe) {
		printk(KERN_INFO "scullpipe not found\n");
		goto ERR;
	}
	if ((filep->f_flags & O_ACCMODE) == O_WRONLY)
		lscullpipe->num_writer--;

	if ((filep->f_flags & O_ACCMODE) == O_RDONLY)
		lscullpipe->num_reader--;

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
ERR:
	return -1;
}
