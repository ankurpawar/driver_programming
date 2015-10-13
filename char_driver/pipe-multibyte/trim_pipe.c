#include"header.h"
#include"declarations.h"

int trim_pipe(struct ScullPipe *lscullpipe)
{
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	if (!lscullpipe) {
		printk(KERN_ERR "no scull pipe found!\n");
		goto ERR;
	}
	
	if (!lscullpipe->buffer) {
		printk(KERN_ERR "no pipe buffer found!\n");
		goto ERR;
	}
	
	kfree(lscullpipe->buffer);
	lscullpipe->buffer_size = 0;
	lscullpipe->rp = NULL;
	lscullpipe->wp = NULL;
	lscullpipe->buffer = NULL;
	lscullpipe->end = NULL;
	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
ERR:
	return -1;
}
