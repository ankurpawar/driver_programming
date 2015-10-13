#include"header.h"

static void __exit cleanup(void)
{
	int ret ;
	unregister_chrdev(major_num,DEV_NAME);
	//if(ret < 0) {
	//	printk(KERN_ERR "error in unregistering device\n");
	//}
	printk(KERN_INFO "good bye world\n");
}

module_exit(cleanup); 
