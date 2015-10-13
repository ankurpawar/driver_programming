#include"header.h"
#include"declarations.h"

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	struct parallel_dev *localdev;
	unsigned char stat_reg = 0, dat_reg = 0;
	unsigned char cont_reg = 0;
	int ret, bytes_read = 0, i;
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

	cont_reg = inb(CONTROL_PORT);
	for (i = 0; i < 2; i++) {
		/*check the busy line.If it is high then break loop.*/
		do {
			stat_reg = inb(STATUS_PORT);
			msleep_interruptible(1);
		}
		while (!(stat_reg & 0x80));

		/*make strobe high*/
		cont_reg |= 0x01;
		outb(cont_reg, CONTROL_PORT);

		/*status reg also contains the nibble in s3-s6 bits.no need to read status reg again*/
		if (i == 0)
			dat_reg = (stat_reg >> 3) & 0x0F; /*lower nibble*/
		else
			dat_reg |= (stat_reg << 1) & 0xF0; /*higher nibble*/
		#ifdef DEBUG
		printk(KERN_INFO "dat_reg=%c %x\n", dat_reg, dat_reg);
		#endif

		/*make strobe low*/
		cont_reg &= 0xFE;
		outb(cont_reg, CONTROL_PORT);
	}

	ret = copy_to_user(buf, &dat_reg, sizeof(char));
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
