#include"header.h"
#include"declarations.h"


int major_num;
unsigned int minor_num;
unsigned int num_dev;
dev_t dev;
dev_t new_dev;
int data_size;
int device_size;
unsigned long port_address;
struct parallel_dev *par_dev;
struct resource *par_port;
int num_address;

module_param(num_dev,uint,S_IRUGO);

struct file_operations fops = {
	.owner = THIS_MODULE,
        .open = open_dev,
        .release = close_dev,
        .write = write_dev,
	.release = close_dev,
};

void init_default(void)
{
	major_num = MAJOR_NUM;
	minor_num = MINOR_NUM;
	port_address = PORT_ADDRESS;
	num_address = NUM_ADDRESS;
	data_size = DATA_SIZE;
	device_size = DEVICE_SIZE;
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
		par_dev[i].data_size = data_size;
		par_dev[i].device_size = device_size;
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

MODULE_LICENSE("GPL");
module_init(initialization);
