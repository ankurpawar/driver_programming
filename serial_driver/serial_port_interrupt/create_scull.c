#include"header.h"
#include"declarations.h"

int create_quantums(int count,struct ScullQset *lscullqset)
{
	int noq,i;
	int k;
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	noq = count / quantum_size;
	if (count % quantum_size)
		noq++;
	
	if (!lscullqset) {
		printk(KERN_ERR "no scullqset present\n");
		goto ERR;
	}
	#ifdef DEBUG
        printk(KERN_INFO "noq=%d\n",noq);
        #endif
	k = 0;
	for (i = 0; i < noq; i++) {
		lscullqset->data[k] = kmalloc(quantum_size,GFP_KERNEL);
		if (!lscullqset->data[k]) {
			printk(KERN_ERR "error in allocating quantum\n");
			goto ERR;
		}
		memset(lscullqset->data[k],'\0',quantum_size);
		if (k == (qset_size - 1)) {
			lscullqset = lscullqset->next;
			k = 0;
		} else
			k++;
	}
	#ifdef DEBUG 
	printk(KERN_INFO " qunatum made = %d\n",i);
	#endif
	
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return i;
ERR:
	return -1;
}

int create_qset(int count,struct ScullQset *lscullqset)
{
	int item,noi;
	int qset_made = 0, i;
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	
	if (!lscullqset) {
		printk(KERN_ERR "no scullqset present\n");
		goto ERR;
	}
	noi = 0;
	item = qset_size * quantum_size;
	noi = count / item;
	if (count%item)
		noi++;
	i = 0;
	while (lscullqset) {
		lscullqset->data = kmalloc(qset_size * sizeof(void*),GFP_KERNEL);
		memset(lscullqset->data,'\0',qset_size * sizeof(void*));
		if (!lscullqset->data) {
			printk(KERN_ERR "error in allocating qset\n");
			goto ERR;
		}
		lscullqset = lscullqset->next;
		i++;
	}	
	/*for (i = 0; i < noi; i++) {
		lscullqset->data = kmalloc(qset_size * sizeof(void*),GFP_KERNEL);
		if (!lscullqset->data) {
			printk(KERN_ERR "error in allocating qset\n");
			goto ERR;
		}
		if (lscullqset->next) {
			lscullqset = lscullqset->next;
		} else if (!lscullqset->next && (i < noi-2)) {
			printk(KERN_ERR "scullqset->next is not present.\n");
			goto ERR;
		}
	}*/
	qset_made = i;
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return qset_made;
ERR:
	return -1;	
}

struct ScullQset *create_scullqset(int count)
{
	struct ScullQset *lscullqset;
	struct ScullQset *first , *last;
	int noi ,item ,i;  
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	noi = 0;
	item = qset_size * quantum_size ;
	noi = count / item; 
	if (count%item)
		noi++;
	first = NULL;
	last = NULL;
	for (i = 0; i < noi; i++) {
		if (i == 0) {
			first = last = kmalloc(sizeof(struct ScullQset),GFP_KERNEL);
			if (!first) {
				printk(KERN_ERR "error in kmalloc,not creating qset\n");
				goto ERR;
			}
			first->next = NULL;
			last->next = NULL;
		} else {
			lscullqset = kmalloc(sizeof(struct ScullQset),GFP_KERNEL);
			if (!lscullqset) {
				printk(KERN_ERR "error in kmalloc,not creating qset\n");
				goto ERR;
			}
			last->next = lscullqset;
			last = lscullqset;
			lscullqset->next = NULL;
		}
	}
	#ifdef DEBUG
        printk(KERN_INFO "no of scullqset created=%d\n",i);
        #endif
	
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return first;
ERR:	
	return NULL;
}

struct ScullQset *create_scull(int count)
{
	struct ScullQset *lscullqset = NULL;
	int ret = 0;
	#ifdef DEBUG
        printk(KERN_INFO "START: %s \n",__func__);
        #endif
	lscullqset = create_scullqset(count);
	if(!lscullqset) {
		printk(KERN_ERR "error in creating scullqset\n");
		goto ERR;
	}
	ret = create_qset(count,lscullqset);
	if (ret < 0) {
		printk(KERN_ERR "error in creating qset\n");
		goto ERR;
	}
	#ifdef DEBUG
        printk(KERN_INFO "number of qset made = %d\n",ret);
        #endif

	ret = create_quantums(count,lscullqset);
	if (ret < 0) {
		printk(KERN_ERR "error in creating qunatums\n");
		goto ERR;
	}
	#ifdef DEBUG
        printk(KERN_INFO "number of qunatum made = %d\n",ret);
        #endif
	
	#ifdef DEBUG
        printk(KERN_INFO "END: %s \n",__func__);
        #endif
	return lscullqset;
ERR:
	return NULL;
}
