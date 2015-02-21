#include"header.h"
#include"declarations.h"

ssize_t read_dev(struct file *filep,char __user *buf,size_t count,loff_t *f_pos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset;
	int byte_pend = 0, to_read, ret, i, bytes_read = 0;
	int qset_num,quant_num,qoff,item,nobtr = 0, k = 0 ,noq;

	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	lsculldev = filep->private_data;
	if (!lsculldev) {
		printk(KERN_ERR "device not found\n");
		return -1;
	}

	wait_for_completion(&lsculldev->my_completion);

	if (*f_pos >= data_size) {
                printk(KERN_ERR "position reached beyond scull\n");
                goto ERR;
        }

	#ifdef DEBUG
	printk(KERN_INFO "number of bytes to read=%u\n",(unsigned int)count);
	#endif	
	
	#ifdef DEBUG
	printk(KERN_INFO "before read,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif
	
	lscullqset = lsculldev->qset;
	if (!lscullqset) {
		printk(KERN_ERR "scull not found\n");
		goto ERR;
	}

	item = quantum_size * qset_size;
	qset_num = *f_pos / item;
	if (*f_pos % item)
		qset_num++;
	
	quant_num = (*f_pos % item) / quantum_size;
	qoff = *f_pos % quantum_size;	

	#ifdef DEBUG
	printk(KERN_INFO "qoff=%d,quant_num=%d,qset_num=%d",qoff,quant_num,qset_num);
	#endif
	
	if ((*f_pos + count) > data_size)	
		nobtr = data_size - *f_pos;
	else
		nobtr = count;
	#ifdef DEBUG
	printk(KERN_INFO "total bytes to read = %d\n",nobtr);
	#endif
	
	to_read = quantum_size;
	bytes_read = 0;
	if (*f_pos == 0)
		i = 0;
	else {
		i = quant_num;
		lscullqset = scull_follow(lscullqset,qset_num - 1);
		if (!lscullqset) {
			printk(KERN_ERR "error in scull follow\n");
			goto ERR;
		}
	}

	noq = nobtr / quantum_size;
	if (nobtr % quantum_size)
		noq++;
	for (k = 0; k < noq; k++) {
		ret = copy_to_user(buf + bytes_read, lscullqset->data[i] + qoff,to_read - qoff);
		if (ret > 0) {
			#ifdef DEBUG
			printk(KERN_INFO "partial read ret=%d\n",ret);
			#endif
		}
		byte_pend = nobtr - bytes_read - to_read + qoff;
		bytes_read = bytes_read + to_read - qoff;
		*f_pos = *f_pos + to_read - qoff;
		if (byte_pend < to_read)
			to_read = byte_pend;
		#ifdef DEBUG
		printk(KERN_INFO "byte_pend = %d,bytes_read=%d\n",byte_pend,bytes_read);
		#endif	
		if (i == (qset_size-1)) {
			lscullqset = lscullqset->next;
			i = 0;	
		} else
			i++;
		#ifdef DEBUG
		printk(KERN_INFO "quantum read=%d\n",i);
		#endif
		qoff = 0;
	}
	filep->f_pos = *f_pos;
	data_size = *f_pos;
		
	#ifdef DEBUG
	printk(KERN_INFO "after read,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif
	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_read;
ERR:
	return -1;
}
