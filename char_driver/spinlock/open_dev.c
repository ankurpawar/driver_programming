#include"header.h"
#include"declarations.h"

int open_dev(struct inode *inodep,struct file *filep)
{
	struct ScullDev *lsculldev;
	int ret;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	lsculldev = (struct ScullDev*)&sculldev[0];
	if(!sculldev) {
		printk(KERN_ERR "sculldev not allocated");
		goto ERR;
	}
	
	lsculldev = container_of(inodep->i_cdev,struct ScullDev,c_dev);
	if (!lsculldev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = lsculldev;
	if((filep->f_flags & O_ACCMODE) == O_WRONLY) {
		#ifdef DEBUG
		printk(KERN_INFO "device write mode\n");
		#endif
		spin_lock_bh(&lsculldev->my_lock);
		ret = trim_dev(lsculldev);
		if(ret < 0) {
			#ifdef DEBUG
			printk(KERN_ERR "device trim failed!\n");
			#endif
			goto COMP;
		}
		spin_unlock_bh(&lsculldev->my_lock);
	}
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
COMP:
	spin_unlock_bh(&lsculldev->my_lock);
ERR:
	return -1;
}
