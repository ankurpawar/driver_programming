#include"header.h"
#include"declarations.h"

static void __exit cleanup_func(void)
{
	int i,ret;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	for (i = 0;i < num_dev ;i++) {
		cdev_del(&scullpipe[i].c_dev);
		ret = trim_pipe(&scullpipe[i]);
		if (ret < 0) {
			#ifdef DEBUG
			printk(KERN_ERR "device trim failed!\n");
			#endif
		}
	}
	kfree(scullpipe);
	unregister_chrdev_region(dev,num_dev);

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
} 

module_exit(cleanup_func);
