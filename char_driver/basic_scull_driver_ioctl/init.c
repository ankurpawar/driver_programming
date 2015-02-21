#include"header.h"
#include"declarations.h"
#include"file_opr.h"


int major_num;
unsigned int minor_num;
unsigned int num_dev;
int qset_size;
int quantum_size;
int device_size;
int data_size;
dev_t dev;
dev_t new_dev;
struct ScullDev *sculldev;

module_param(num_dev,uint,S_IRUGO);

void init_default(void)
{
	major_num = MAJOR_NUM;
	minor_num = MINOR_NUM;
	qset_size = QSET_SIZE;
	quantum_size = QUANTUM_SIZE;
	data_size = DATA_SIZE;
	device_size = DEVICE_SIZE;
}

static int __init initialization(void)
{
	int ret,i;
	unsigned int ma,mi;
	struct proc_dir_entry *proc_dir = NULL;

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
	
	proc_dir = proc_create("scullmem",0,NULL,&scull_proc_ops);
	if (!proc_dir) {
		printk(KERN_ERR "error in creating proc entry\n");
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
