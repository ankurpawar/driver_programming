#include"header.h"
#include"declarations.h"

loff_t llseek_dev(struct file *filep,loff_t offset,int origin)
{
	loff_t fpos = 0;
	struct ScullDev *lsculldev = filep->private_data;
	#ifdef DEBUG
	printk("START : %s\n",__func__);
	#endif
	if (!lsculldev) {
		printk(KERN_ERR "no device found!\n");
		return -1;
	}

	spin_lock_bh(&lsculldev->my_lock);

	if (offset > device_size) {
		printk(KERN_ERR "offset beyond device_size!\n");
		goto ERR;
	}
	
	switch (origin) {
	case SEEK_SET :
		fpos = offset;
		filep->f_pos = offset;
		break;
	case SEEK_CUR :
		break;
	case SEEK_END :
		break;
	}
	#ifdef DEBUG
	printk(KERN_INFO "fpos = %d\n",(int)fpos);
	#endif
	spin_unlock_bh(&lsculldev->my_lock);
	#ifdef DEBUG
	printk("END : %s\n",__func__);
	#endif
	return fpos;
ERR:
	spin_unlock_bh(&lsculldev->my_lock);
	return -1;
}
