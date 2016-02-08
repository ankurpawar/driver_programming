#include "serial.h"

#define TX_FLAGS (THRE|TEMT)

struct serial_dev
{
        struct cdev c_dev;
        struct semaphore sem;
};

int major_num;
unsigned int minor_num;
unsigned int num_dev;
dev_t dev;
dev_t new_dev;
unsigned long port_address;
struct serial_dev *serialdev;
struct resource *serial_port;
int num_regs;

module_param(num_dev,uint,S_IRUGO);
MODULE_LICENSE("GPL");

int open_dev(struct inode *,struct file *);
int close_dev(struct inode *,struct file *);
ssize_t write_dev(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_dev(struct file *,char __user *,size_t count,loff_t *);

struct file_operations fops=
{
	.open = open_dev,
	.release = close_dev,
	.read = read_dev,
	.write = write_dev
};

void init_default(void)
{
	major_num = MAJOR_NUM;
	minor_num = MINOR_NUM;
	port_address = PORT_ADDRESS;
	num_regs = NUM_REGS;
}

static int __init initialization(void)
{
	int ret,i;
	unsigned int ma,mi;
	unsigned char lcr = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	init_default();
	serial_port = NULL;
	ret = alloc_chrdev_region(&dev,minor_num,num_dev,DEV_NAME);
	if(ret < 0) {
		printk(KERN_ERR "registration failure\n");
		goto ERR;
	}
	#ifdef DEBUG
	printk(KERN_INFO "major number = %u ,minor number=%u\n",MAJOR(dev),MINOR(dev));
	#endif
	
	serialdev = kmalloc(sizeof(struct serial_dev)*num_dev,GFP_KERNEL);
	if(!serialdev) {
		printk(KERN_ERR "error in kmalloc\n");
		goto ERR;
	}

	memset(serialdev,'\0',sizeof(struct serial_dev) * num_dev);
	
	ma = MAJOR(dev);

	for(i=0;i < num_dev ;i++) {
		mi = MINOR(dev+i);
		new_dev = MKDEV(ma,mi);
		
		cdev_init(&serialdev[i].c_dev,&fops);
		serialdev[i].c_dev.owner = THIS_MODULE;
		serialdev[i].c_dev.ops = &fops;
		sema_init(&serialdev[i].sem,1);
		ret = cdev_add(&serialdev[i].c_dev,new_dev,1);
		if(ret != 0) {
			printk(KERN_ERR "error in adding cdev\n");
			goto ERR;
		}
		#ifdef DEBUG
		printk(KERN_INFO "cdev major num = %d,minor_num = %d \n",MAJOR(serialdev[i].c_dev.dev),MINOR(serialdev[i].c_dev.dev));
		#endif
	}

	if (check_region(port_address,num_regs) < 0)
		release_region(port_address,num_regs);
	serial_port = request_region(port_address,num_regs,DEV_NAME);
	if (serial_port == NULL) {
		printk(KERN_ERR "error in acquiring serial port\n");
		goto ERR;
	}

	
	/*initialize required registers*/
	lcr |= DLAB;
	outb(lcr,LCR);
	outb(0x0C,DLL);
	outb(0x00,DLM);
	lcr &= (~DLAB);
	lcr |= 0x1B;
	outb(lcr,LCR);
	outb(0x00,MCR);
	outb(0x00,SCR);
	outb(0x00,FCR);
	outb(0x00,IER);
	


	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);	
	#endif
	return 0;
ERR:	
	return -1; 
}
module_init(initialization);


static void __exit cleanup_func(void)
{
	int i;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	
	for (i = 0;i < num_dev ;i++) {
		cdev_del(&serialdev[i].c_dev);
	}
	if (serial_port)
		release_region(port_address,num_regs);
	kfree(serialdev);
	unregister_chrdev_region(dev,num_dev);

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
} 
module_exit(cleanup_func);

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *fpos)
{
	struct serial_dev *lserialdev;
	unsigned char inbyte = 0;
	int ret, bytes_read = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	lserialdev = filep->private_data;
	if (!lserialdev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}

	do {
		inbyte = inb(LSR);
		udelay(1);
	} while ((inbyte & DR) != DR);

	/*read data from serial port*/
	inbyte = inb(RBR);
	
	/*copy data to user space application*/
	ret = copy_to_user(buf, &inbyte, sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial read ret=%d\n", ret);
		#endif
	}
	printk(KERN_INFO "byte read=%d\n", inbyte);

	bytes_read = 1;
	return bytes_read;
ERR:
	return -1;
}

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct serial_dev *lserialdev;
	unsigned char inbyte = 0;
	unsigned char outbyte = 0;
	int ret, bytes_wrtn = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	lserialdev = filep->private_data;
	if (!lserialdev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}
	
	/*write data to port*/
	ret = copy_from_user(&outbyte, buf, sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial write ret=%d\n", ret);
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "data to port =  %c \n", outbyte);
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

int open_dev(struct inode *inodep, struct file *filep)
{
	struct serial_dev *lserialdev;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	
	lserialdev = container_of(inodep->i_cdev, struct serial_dev, c_dev);
	if (!lserialdev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = lserialdev;
	if(down_interruptible(&lserialdev->sem))
		return -ERESTARTSYS;
	
	up(&lserialdev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
SEM_UP:
	up(&lserialdev->sem);
ERR:
	return -1;
}

int close_dev(struct inode *inodep,struct file *filep)
{
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
}
