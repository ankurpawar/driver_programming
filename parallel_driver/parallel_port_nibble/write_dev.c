#include"header.h"
#include"declarations.h"

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	unsigned char dat_reg, inbyte = 0;
	unsigned char outbyte = 0;
	int ret, bytes_wrtn = 0, i;
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

	ret = copy_from_user(lscullqset->data[0], buf, sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial write ret=%d\n",ret);
		#endif
	}
	memcpy(&dat_reg,lscullqset->data[0],sizeof(char));
	#ifdef DEBUG
	printk(KERN_INFO "data to port =  %c \n",outbyte);
	#endif 
	
	for (i = 0; i < 2; i++) {
		if (i == 0)
			outb(dat_reg, DATA_PORT); /*lower nibble*/
		else
			outb(dat_reg>>4, DATA_PORT); /*higher nibble*/

		/*check the busy line*/
		do {
			inbyte = inb(STATUS_PORT);
			msleep_interruptible(1);
		} while (inbyte & 0x80);

		/*assert strobe high then low*/
		outb(0x01, CONTROL_PORT);
		msleep_interruptible(1);
		outb(0x00, CONTROL_PORT);
	}

	bytes_wrtn = 1;
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_wrtn;
ERR:
	return -1;
}
