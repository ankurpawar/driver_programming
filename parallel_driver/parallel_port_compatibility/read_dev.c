#include"header.h"
#include"declarations.h"

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	unsigned char inbyte = 0;
	unsigned char outbyte = 0;
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
	/*make the data reg as input*/
	outbyte = 32; 
	outb(outbyte,CONTROL_PORT);
	memcpy(lscullqset->data[2],&outbyte,sizeof(char));
	/*check the busy line if high then break loop and read*/
	do {
		inbyte = inb(STATUS_PORT);
		msleep_interruptible(1);
	}
	while (!(inbyte & 0x80));

	/*make strobe high*/
	outbyte |= 0x1;
	outb(outbyte,CONTROL_PORT);
	memcpy(lscullqset->data[2],&outbyte,sizeof(char));

	/*read data*/
	inbyte = inb(DATA_PORT);

	/*make strobe low*/
	outbyte &= 0xFE;
	outb(outbyte, CONTROL_PORT);
	memcpy(lscullqset->data[2], &outbyte, sizeof(char));

	#ifdef DEBUG
	printk(KERN_INFO "data read=%c %x\n",inbyte,inbyte);
	#endif
	memcpy(lscullqset->data[0],&inbyte,sizeof(char));
	
	ret = copy_to_user(buf,lscullqset->data[0], sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial write ret=%d\n",ret);
		#endif
	}
	bytes_read = 1;
	return bytes_read;
ERR:
	return -1;
}
