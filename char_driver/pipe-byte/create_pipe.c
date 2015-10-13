#include"header.h"
#include"declarations.h"

char *create_pipe(int count)
{
	char *buffer = NULL;
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	
	if (count <= 0) {
        	printk(KERN_ERR "pipe size cant be zero or negative %d\n",count);
        	goto ERR; 
	}
	
	buffer = kmalloc(sizeof(char) * count,GFP_KERNEL);
	if (!buffer) {
		printk(KERN_ERR "error in kmalloc,not allocating buffer\n");
		goto ERR;
	}
	
	#ifdef DEBUG
        printk(KERN_INFO "created a pipe of size = %d\n",count*sizeof(char));
        #endif

	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return buffer;
ERR:
	return NULL;
}
