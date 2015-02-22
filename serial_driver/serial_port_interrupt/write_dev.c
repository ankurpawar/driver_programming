#include"header.h"
#include"declarations.h"
#define TX_FLAGS (THRE|TEMT)

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

	lscullqset = lsculldev->qset;
	if (!lsculldev->qset) {
		printk(KERN_ERR "error scull not found\n");
		goto ERR;
	}
	
	/*write data to port*/
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
	
	outb(outbyte, THR);

	do {
		inbyte = inb(LSR);
		udelay(1);
	} while((inbyte & TX_FLAGS) != TX_FLAGS);

	bytes_wrtn = 1;
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_wrtn;
ERR:
	return -1;
}
