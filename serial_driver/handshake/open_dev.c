#include"header.h"
#include"declarations.h"

int open_dev(struct inode *inodep, struct file *filep)
{
	struct ScullDev *lsculldev;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	lsculldev = (struct ScullDev*)&sculldev[0];
	if(!sculldev) {
		printk(KERN_ERR "sculldev not allocated");
		goto ERR;
	}
	
	lsculldev = container_of(inodep->i_cdev, struct ScullDev, c_dev);
	if (!lsculldev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = lsculldev;
	if(down_interruptible(&lsculldev->sem))
		return -ERESTARTSYS;
	
	lsculldev->qset = create_scull(num_regs);
	if (!lsculldev->qset) {
		printk(KERN_ERR "error in creating scull\n");
		goto SEM_UP;
	}
	up(&lsculldev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
SEM_UP:
	up(&lsculldev->sem);
ERR:
	return -1;
}


int trim_dev(struct ScullDev *lsculldev)
{
	struct ScullQset *lqset;
	struct ScullQset *next = NULL;
	int i;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	if (!lsculldev) {
		printk(KERN_ERR "no device found!\n");
		goto ERR;
	}

	if (!lsculldev->qset) {
		#ifdef DEBUG
		printk(KERN_INFO "no qset found.\n");
		#endif	
		goto RET;
	}

	lqset = lsculldev->qset;
	for (lqset = lsculldev->qset; lqset; lqset = next) {
		if (lqset->data) {
			for (i = 0; i < qset_size ; i++)
				kfree(lqset->data[i]);
			kfree(lqset->data);
			lqset->data = NULL;
		}
		next = lqset->next;
		kfree(lqset);
	}
	lsculldev->qset = NULL;
	lsculldev->data_size = 0;	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
RET:
	return 0;
ERR:
	return -1;
}
