#include"header.h"
#include"declarations.h"


int major_num;
unsigned int minor_num;
unsigned int num_dev;
int qset_size;
int quantum_size;
dev_t dev;
dev_t new_dev;
int data_size;
int device_size;
unsigned long port_address;
struct ScullDev *sculldev;
struct resource *par_port;
int num_regs;



struct file_operations fops=
{
	open:open_dev,
	release:close_dev,
	read:read_dev,
	write:write_dev
};

void init_default(void)
{
	major_num = MAJOR_NUM;
	minor_num = MINOR_NUM;
	qset_size = QSET_SIZE;
	quantum_size = QUANTUM_SIZE;
	port_address = PORT_ADDRESS;
	num_regs = NUM_REGS;
	data_size = DATA_SIZE;
	device_size = DEVICE_SIZE;
	num_dev = NOD;
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
	par_port = NULL;
	ret = alloc_chrdev_region(&dev,minor_num,num_dev,DEV_NAME);
	if(ret < 0) {
		printk(KERN_ERR "registration failure\n");
		goto ERR;
	}
	#ifdef DEBUG
	printk(KERN_INFO "major number = %u ,minor number=%u\n",MAJOR(dev),MINOR(dev));
	#endif
	
	sculldev = kmalloc(sizeof(struct ScullDev)*num_dev,GFP_KERNEL);
	if(!sculldev) {
		printk(KERN_ERR "error in kmalloc\n");
		goto ERR;
	}

	memset(sculldev,'\0',sizeof(struct ScullDev) * num_dev);
	
	ma = MAJOR(dev);

	for(i=0;i < num_dev ;i++) {
		mi = MINOR(dev+i);
		new_dev = MKDEV(ma,mi);
		
		cdev_init(&sculldev[i].c_dev,&fops);
		sculldev[i].c_dev.owner = THIS_MODULE;
		sculldev[i].c_dev.ops = &fops;
		sculldev[i].qset_size = qset_size;
		sculldev[i].quantum_size = quantum_size;
		sculldev[i].data_size = data_size;
		sculldev[i].device_size = device_size;
		sema_init(&sculldev[i].sem,1);
		ret = cdev_add(&sculldev[i].c_dev,new_dev,1);
		if(ret != 0) {
			printk(KERN_ERR "error in adding cdev\n");
			goto ERR;
		}
		#ifdef DEBUG
		printk(KERN_INFO "cdev major num = %d,minor_num = %d \n",MAJOR(sculldev[i].c_dev.dev),MINOR(sculldev[i].c_dev.dev));
		#endif
	}

	if (check_region(port_address,num_regs) < 0)
		release_region(port_address,num_regs);
	par_port = request_region(port_address,num_regs,DEV_NAME);
	if (par_port == NULL) {
		printk(KERN_ERR "error in acquiring serial port\n");
		goto ERR;
	}

	
	/*initialize required registers*/
	lcr |= LCR_DLAB;
	outb(lcr, LCR);
	outb(0x0C, DLL);
	outb(0x00, DLM);
	lcr &= (~LCR_DLAB);
	lcr |= (LCR_EPS | LCR_PEN | LCR_WLS_8);
	outb(lcr, LCR);
	outb(0x00, MCR);
	outb(0x00, SCR);
	outb(0x00, FCR);
	outb(0x00, IER);
	


	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);	
	#endif
	return 0;
ERR:	
	return -1; 
}

MODULE_LICENSE("GPL");
module_init(initialization);
