#include"header.h"
#include"declarations.h"

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	struct parallel_dev *localdev;
	unsigned char inbyte = 0;
	unsigned char outbyte = 0;
	int ret, bytes_read = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	localdev = filep->private_data;
	if (!localdev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}

	if (count > device_size) {
		printk(KERN_ERR "data too large\n");
		goto ERR;
	}

	/*make the data reg as input*/
	outbyte = 32; 
	outb(outbyte,CONTROL_PORT);
	/*check the busy line if high then break loop and read*/
	do {
		inbyte = inb(STATUS_PORT);
		msleep_interruptible(1);
	}
	while (!(inbyte & 0x80));

	/*make strobe high*/
	outbyte |= 0x1;
	outb(outbyte,CONTROL_PORT);

	/*read data*/
	inbyte = inb(DATA_PORT);

	/*make strobe low*/
	outbyte &= 0xFE;
	outb(outbyte, CONTROL_PORT);

	#ifdef DEBUG
	printk(KERN_INFO "data read=%c %x\n",inbyte,inbyte);
	#endif
	
	ret = copy_to_user(buf,&inbyte, sizeof(char));
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
