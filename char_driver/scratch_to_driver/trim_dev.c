#include"header.h"
#include"declarations.h"

int trim_scullqset(struct ScullQset *lscullqset)
{
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	if(!lscullqset) {
		printk(KERN_ERR "scullqset empty \n");
		return -1;
	}

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
}

int trim_dev(struct ScullDev *lsculldev)
{
	struct ScullQset *lqset;
	struct ScullQset *prev_qset = NULL;
	int ret,flag;

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
	while (lqset->next) {
		flag = 1;
		if (flag)
			prev_qset = lqset;
		lqset = lqset->next;
	}

	if (!lqset->data) {
		printk(KERN_INFO "no qset array\n");
		ret = trim_scullqset(lqset);
		if (ret < 0) { 
			printk(KERN_ERR "error in trimming scullqset\n");
			goto ERR;
		}
		if (flag == 1)
			prev_qset->next = NULL; 	
	}

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
RET:
	return 0;
ERR:
	return -1;
}
