#include"header.h"
#include"declarations.h"

int major_num;
unsigned int minor_num;
unsigned int num_dev;
dev_t dev;
dev_t new_dev;
struct ScullPipe *scullpipe;
int max_pipe_size;

module_param(num_dev,uint,S_IRUGO);

struct file_operations fops = {
        .owner = THIS_MODULE,
	.open = open_pipe,
        .release = close_pipe,
        .write = write_pipe,
        .read = read_pipe,
};

void init_default(void)
{
	major_num = MAJOR_NUM;
	minor_num = MINOR_NUM;
	max_pipe_size = MAX_BUFFER_SIZE;
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
	
	scullpipe = kmalloc(sizeof(struct ScullPipe)*num_dev,GFP_KERNEL);
	if(!scullpipe) {
		printk(KERN_ERR "error in kmalloc\n");
		goto ERR;
	}

	memset(scullpipe,'\0',sizeof(struct ScullPipe) * num_dev);
	
	ma = MAJOR(dev);

	for(i=0;i < num_dev ;i++) {
		mi = MINOR(dev+i);
		new_dev = MKDEV(ma,mi);
		
		cdev_init(&scullpipe[i].c_dev,&fops);
		scullpipe[i].c_dev.owner = THIS_MODULE;
		scullpipe[i].c_dev.ops = &fops;
		scullpipe[i].buffer_size = 0;
		scullpipe[i].rp = NULL;
		scullpipe[i].wp = NULL;
		scullpipe[i].buffer = NULL;
		scullpipe[i].end = NULL;
		sema_init(&scullpipe[i].sem,1);
		ret = cdev_add(&scullpipe[i].c_dev,new_dev,1);
		init_waitqueue_head(&scullpipe[i].inq);
		init_waitqueue_head(&scullpipe[i].outq);
		if(ret != 0) {
			printk(KERN_ERR "error in adding cdev\n");
			goto ERR;
		}
		#ifdef DEBUG
		printk(KERN_INFO "cdev major num = %d,minor_num = %d \n",MAJOR(scullpipe[i].c_dev.dev),MINOR(scullpipe[i].c_dev.dev));
		#endif
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
