#include"header.h"
#include"declarations.h"

int trim_dev(struct ScullDev *lsculldev)
{
	struct ScullQset *lqset;
	struct ScullQset *next = NULL;
	int i;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	if (!lsculldev) {
		printk(KERN_ERR "no scull found!\n");
		goto ERR;
	}

	if (!lsculldev->qset) {
		#ifdef DEBUG
		printk(KERN_INFO "no qset found.\n");
		#endif	
		goto RET;
	}

	lqset = lsculldev->qset;
	for (lqset = lsculldev->qset; lqset; lqset = next) {
		if (lqset->data) {
			for (i = 0; i < qset_size ; i++)
				kfree(lqset->data[i]);
			kfree(lqset->data);
			lqset->data = NULL;
		}
		next = lqset->next;
		kfree(lqset);
	}
	lsculldev->qset = NULL;
	atomic_read(&lsculldev->data_size);	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
RET:
	return 0;
ERR:
	return -1;
}
