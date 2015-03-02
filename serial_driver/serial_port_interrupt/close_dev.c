#include"header.h"
#include"declarations.h"

int close_dev(struct inode *inodep,struct file *filep)
{
	unsigned char byte = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	byte = 0;
	outb(byte, IER);
	free_irq(irq, DEV_NAME);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
}
