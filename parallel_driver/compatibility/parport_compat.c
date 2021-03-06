#include "parport_compat.h"

struct parallel_dev
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
struct parallel_dev *par_dev;
struct resource *par_port;
int num_address;

module_param(num_dev,uint,S_IRUGO);
MODULE_LICENSE("GPL");

int open_dev(struct inode *,struct file *);
int close_dev(struct inode *,struct file *);
ssize_t write_dev(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_dev(struct file *,char __user *,size_t count,loff_t *);

struct file_operations fops = {
	.owner = THIS_MODULE,
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
	num_address = NUM_ADDRESS;
}

static int __init initialization(void)
{
	int ret,i;
	unsigned int ma,mi;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	init_default();
	
	ret = alloc_chrdev_region(&dev,minor_num,num_dev,DEV_NAME);
	if(ret < 0) {
		printk(KERN_ERR "registration failure\n");
		goto ERR;
	}
	#ifdef DEBUG
	printk(KERN_INFO "major number = %u ,minor number=%u\n",MAJOR(dev),MINOR(dev));
	#endif
	
	par_dev = kmalloc(sizeof(struct parallel_dev)*num_dev,GFP_KERNEL);
	if(!par_dev) {
		printk(KERN_ERR "error in kmalloc\n");
		goto ERR;
	}

	memset(par_dev,'\0',sizeof(struct parallel_dev) * num_dev);
	
	ma = MAJOR(dev);

	for(i=0;i < num_dev ;i++) {
		mi = MINOR(dev+i);
		new_dev = MKDEV(ma,mi);
		
		cdev_init(&par_dev[i].c_dev,&fops);
		par_dev[i].c_dev.owner = THIS_MODULE;
		par_dev[i].c_dev.ops = &fops;
		sema_init(&par_dev[i].sem,1);
		ret = cdev_add(&par_dev[i].c_dev,new_dev,1);
		if(ret != 0) {
			printk(KERN_ERR "error in adding cdev\n");
			goto ERR;
		}
		#ifdef DEBUG
		printk(KERN_INFO "cdev major num = %d,minor_num = %d \n",MAJOR(par_dev[i].c_dev.dev),MINOR(par_dev[i].c_dev.dev));
		#endif
	}

	if (check_region(port_address,num_address) < 0)
		release_region(port_address,num_address);
	par_port = request_region(port_address,num_address,DEV_NAME);
	if (par_port == NULL) {
		printk(KERN_ERR "error in getting parallel port\n");
		goto ERR;
	}

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
		cdev_del(&par_dev[i].c_dev);
	}
	release_region(port_address,3);
	kfree(par_dev);
	unregister_chrdev_region(dev,num_dev);

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
}
module_exit(cleanup_func);

int open_dev(struct inode *inodep, struct file *filep)
{
	struct parallel_dev *localdev;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	
	localdev = container_of(inodep->i_cdev, struct parallel_dev, c_dev);
	if (!localdev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = localdev;
	if(down_interruptible(&localdev->sem))
		return -ERESTARTSYS;
	
	up(&localdev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
SEM_UP:
	up(&localdev->sem);
ERR:
	return -1;
}

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct parallel_dev *localdev;
	unsigned char inbyte = 0;
	unsigned char outbyte = 0;
	int ret, bytes_wrtn = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	localdev = filep->private_data;
	if (!localdev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}
	
	/*write data to port*/
	ret = copy_from_user(&outbyte, buf, sizeof(char));
	if (ret > 0) {
		#ifdef DEBUG
		printk(KERN_INFO "partial write ret=%d\n",ret);
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "data to port =  %c \n",outbyte);
	#endif 
	outb(outbyte,DATA_PORT);

	/*check the busy line*/
	do {
		inbyte = inb(STATUS_PORT);
		msleep_interruptible(1);
	} while (inbyte & 0x80);

	/*assert strobe high then low*/
	outb(0x01,CONTROL_PORT);
	msleep_interruptible(1);
	outb(0x00,CONTROL_PORT);
	
	bytes_wrtn = 1;
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_wrtn;
ERR:
	return -1;
}

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
