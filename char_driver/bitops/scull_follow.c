#include"header.h"
#include"declarations.h"

struct ScullQset *scull_follow(struct ScullQset *lscullqset,int qset_num)
{
	int i = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	if (!lscullqset) {
		printk(KERN_ERR "no scullqset\n");
		goto ERR;
	}
	while (lscullqset->next && i < qset_num) {
		lscullqset = lscullqset->next;
		i++;
	}
	#ifdef DEBUG
	printk(KERN_INFO "scull followed to %d\n",i);
	#endif
	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return lscullqset;
ERR:
	return NULL;
}
