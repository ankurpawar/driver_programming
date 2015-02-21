#include"header.h"
#include"declarations.h"

int expand_scull(struct ScullQset *lscullqset,int alloc_bytes)
{
	int noqset = 0,item = 0,noquant = 0;
	int k = 0,quant_req = 0,i;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	if(!lscullqset) {
		printk(KERN_ERR "no qset found\n");
		goto ERR;
	}
	
	#ifdef DEBUG
	printk(KERN_INFO "alloc_bytes = %d\n",alloc_bytes);
	#endif

	item = quantum_size * qset_size;
	noqset = data_size / item;
	if (data_size % item)
		noqset++;
	
	k = data_size % item;
	noquant = k / quantum_size;
	if (k % quantum_size)
		noquant++;

	quant_req = alloc_bytes / quantum_size;
	if (alloc_bytes % quantum_size)
		quant_req++;
	#ifdef DEBUG
	printk(KERN_INFO "quant_req = %d\n",quant_req);
	#endif

	lscullqset = scull_follow(lscullqset,noqset);
	if (!lscullqset) {
		printk(KERN_ERR "error in following scull\n");
		goto ERR;
	}

	/*we are in the last scullqset.If this has empty qset index then allocate quantum on those index
	and decrement number of quantums to be allocated*/
	for (i = 0;i < qset_size; i++) {
		if(!lscullqset->data[i]) {
			#ifdef DEBUG
			printk(KERN_INFO "extra quant i = %d\n",i);
			#endif
			lscullqset->data[i] = kmalloc(quantum_size,GFP_KERNEL);
	                if (!lscullqset->data[i]) {
        	                printk(KERN_ERR "error in allocating quantum\n");
                	        goto ERR;
                	}
                	memset(lscullqset->data[i],'\0',quantum_size);
			quant_req--;
		}
	}
	#ifdef DEBUG
	printk(KERN_INFO "qunatum left = %d\n",quant_req);
	#endif
	
	/*Allocate the remanining quantum in a new scullqset which will be added in the previous 
          scullqset*/
	if (quant_req > 0) {
		lscullqset->next = create_scull(quant_req * quantum_size);
		if (!lscullqset->next) { 
			printk(KERN_ERR "error in creating_scull\n");
			goto ERR;
		}
	}
	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return 0;
ERR:
	return -1;
}
