#include"header.h"
#include"declarations.h"

static void __exit cleanup_func(void)
{
	int i;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	for (i = 0;i < num_dev ;i++) {
		trim_dev(&sculldev[i]);
		cdev_del(&sculldev[i].c_dev);
	}

	kfree(sculldev);
	unregister_chrdev_region(dev,num_dev);
	remove_proc_entry("scullmem",NULL);
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
} 

module_exit(cleanup_func);
