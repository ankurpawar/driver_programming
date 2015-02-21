#include"header.h"
#include"declarations.h"

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	unsigned char inbyte = 0;
	unsigned char outbyte = 0;
	int ret, bytes_wrtn = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	lsculldev = filep->private_data;
	if (!lsculldev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}
	
	if (count > device_size) {
		printk(KERN_ERR "data too large\n");
		goto ERR;
	}

	lsculldev->qset = create_scull(count);
	if (!lsculldev->qset) {
		printk(KERN_ERR "error in creating scull\n");
		goto ERR;
	}
	lscullqset = lsculldev->qset;
	
	ret = copy_from_user(lscullqset->data[0], buf, sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial write ret=%d\n",ret);
		#endif
	}
	memcpy(&outbyte,lscullqset->data[0],sizeof(char));
	#ifdef DEBUG
	printk(KERN_INFO "data to port =  %c \n",outbyte);
	#endif 
	outb(outbyte,DATA_PORT);
	bytes_wrtn = 1;

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_wrtn;
ERR:
	return -1;
}
