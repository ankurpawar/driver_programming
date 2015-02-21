#include"header.h"
#include"declarations.h"

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	int byte_pend, to_write, ret, i, bytes_wrtn = 0;
	int qset_num,quant_num,qoff,item;
	int quant_empty = 0,alloc_bytes = 0;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	lsculldev = filep->private_data;
	if (!lsculldev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}
	
	if(down_interruptible(&lsculldev->sem))
		return -ERESTARTSYS;	

	if (count > device_size) {
		printk(KERN_ERR "data too large\n");
		goto ERR;
	}

	if(!buf) {
		printk(KERN_ERR "buffer pointing to NULL.\n");
		goto ERR;
	}
	
	item = quantum_size * qset_size;
	qset_num = *f_pos / item;
	if (*f_pos % item)
		qset_num++;
	
	quant_num = (*f_pos % item) / quantum_size;
	qoff = *f_pos % quantum_size;	

	#ifdef DEBUG
	printk(KERN_INFO "qoff=%d,qunat_num=%d,qset_num=%d\n",qoff,quant_num,qset_num);
	#endif
	
	#ifdef DEBUG
	printk(KERN_INFO "number of bytes to write=%u\n",(unsigned int)count);
	#endif	

	#ifdef DEBUG
	printk(KERN_INFO "before write,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif

	byte_pend = count;
	to_write = quantum_size;
	if (*f_pos == 0)
		i = 0;
	else
		i = quant_num;

	#ifdef DEBUG
	printk(KERN_INFO "i = %d \n",i);
	#endif
	
	/*If qset is empty in sculldev then create scull.Otherwise check condition to extend scull.
	  If required then allocate memory for new qset and qunatum*/
	if (!lsculldev->qset) {
		lsculldev->qset = create_scull(count);
		if (!lsculldev->qset) {
			printk(KERN_ERR "error in creating scull\n");
			goto ERR;
		}
		lscullqset = lsculldev->qset;
	} else if ((*f_pos+count) > data_size) {
		quant_empty = data_size % quantum_size;
		alloc_bytes = (*f_pos + count) - data_size - quant_empty;
		/*we will allocate these number of bytes using expand_scull function.*/
		ret = expand_scull(lsculldev->qset,alloc_bytes);
		if (ret < 0) {
			printk(KERN_ERR "error in expanding scull\n");
			goto ERR;
		}
		
		/*start writting from fpos whether data is present there or not.
		it would be overwritten.use scull_follow to get the correct qset to write*/
		lscullqset = scull_follow(lsculldev->qset,qset_num - 1);
		if (!lscullqset) {
			printk(KERN_ERR "error in following the scull\n");
			goto ERR;
		}
	} else if ((*f_pos + count) < data_size) {
		
		lscullqset = scull_follow(lsculldev->qset,qset_num);
		if (!lscullqset) {
			printk(KERN_ERR "error in following the scull\n");
			goto ERR;
		}
	}
		

	while (bytes_wrtn < count) {
		#ifdef DEBUG
		printk(KERN_INFO "data[i] = %p\n",lscullqset->data[i]);
		#endif
		ret = copy_from_user(lscullqset->data[i] + qoff,buf + bytes_wrtn,to_write - qoff);
		if (ret > 0) {
			#ifdef DEBUG
			printk(KERN_INFO "partial write ret=%d\n",ret);
			#endif
		}
		#ifdef DEBUG
		printk(KERN_INFO "data written = %s\n",(char *)lscullqset->data[i]);
		#endif
		byte_pend = count - bytes_wrtn - to_write + qoff;
		bytes_wrtn = bytes_wrtn + to_write - qoff;
		*f_pos = *f_pos + to_write - qoff;
		if (byte_pend < to_write) 
			to_write = byte_pend;
		#ifdef DEBUG
		printk(KERN_INFO "byte_pend = %d,to_write=%d\n",byte_pend,to_write);
		#endif		
		if (i == (qset_size-1)) {
			lscullqset = lscullqset->next;
			i = 0;	
		} else
			i++;
		#ifdef DEBUG
		printk(KERN_INFO "quantum written=%d\n",i);
		#endif
		qoff = 0;
	}

	if (*f_pos > data_size)
		data_size = *f_pos;
	else if (data_size == 0)
		data_size = bytes_wrtn;

	filep->f_pos = *f_pos;
	up(&lsculldev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "after write,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_wrtn;
ERR:
	up(&lsculldev->sem);
	return -1;
} 

