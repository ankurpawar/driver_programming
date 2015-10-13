#include"header.h"
#include"declarations.h"

int open_dev(struct inode *inodep, struct file *filep)
{
	struct parallel_dev *localdev;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	localdev = container_of(inodep->i_cdev, struct parallel_dev, c_dev);
	if (!localdev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = localdev;
	if(down_interruptible(&localdev->sem))
		return -ERESTARTSYS;
	
	up(&localdev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
SEM_UP:
	up(&localdev->sem);
ERR:
	return -1;
}
