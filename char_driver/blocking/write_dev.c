#include"header.h"
#include"declarations.h"

ssize_t write_dev(struct file *filep,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset = NULL;
	int byte_pend, to_write, ret, i = 0, bytes_wrtn = 0;
	int noq = 0,k;
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

	#ifdef DEBUG
	printk(KERN_INFO "number of bytes to write=%u\n",(unsigned int)count);
	#endif	

	#ifdef DEBUG
	printk(KERN_INFO "before write,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif

	lsculldev->qset = create_scull(count);
	if (!lsculldev->qset) {
		printk(KERN_ERR "error in creating scull\n");
		goto ERR;
	}
	lscullqset = lsculldev->qset;
	noq = count / quantum_size;
	
	if (count % quantum_size)
		noq++;
	
	byte_pend = count;
	to_write = quantum_size;
		
	for (k = 0; k < noq ; k++) {
		ret = copy_from_user(lscullqset->data[i], buf + bytes_wrtn, to_write);
		if (ret > 0) {
			#ifdef DEBUG
			printk(KERN_INFO "partial write ret=%d\n",ret);
			#endif
		}
		#ifdef DEBUG
		printk(KERN_INFO "data written = %s\n",(char *)lscullqset->data[i]);
		#endif
		byte_pend = count - bytes_wrtn - to_write;
		bytes_wrtn = bytes_wrtn + to_write;
		*f_pos = *f_pos + to_write;
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
		printk(KERN_INFO "quantum written=%d\n",k);
		#endif
	}
	data_size = lsculldev->data_size= bytes_wrtn;
	filep->f_pos = *f_pos;
	up(&lsculldev->sem);
	wake_up_interruptible(&lsculldev->dev_wait_q);
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

