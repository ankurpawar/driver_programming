#include"header.h"
#include"declarations.h"

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	unsigned char inbyte = 0, outbyte = 0;
	int ret, bytes_read = 0;
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
		printk(KERN_ERR "error no scull found\n");
		goto ERR;
	}

	/*set RTS bit*/
	outbyte = 0;
	outbyte |= MCR_RTS; 
	outb(outbyte, MCR);

	do {
		inbyte = inb(LSR);
		udelay(1);
	} while ((inbyte & LSR_DR) != LSR_DR);

	inbyte = inb(RBR);
	/*write data to port*/
	memcpy(lscullqset->data[0], &inbyte, sizeof(char));
	ret = copy_to_user(buf, lscullqset->data[0], sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial read ret=%d\n",ret);
		#endif
	}
	printk(KERN_INFO "byte read=%d\n",inbyte);
	
	/*reset RTS bit*/
	outbyte = 0;
	outbyte &= (~MCR_RTS); 
	outb(outbyte, MCR);

	bytes_read = 1;
	return bytes_read;
ERR:
	return -1;
}
