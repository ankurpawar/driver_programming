#include "header.h"

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
	lsculldev->data_size = 0;	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
RET:
	return 0;
ERR:
	return -1;
}

int open_dev(struct inode *inodep,struct file *filep)
{
	struct ScullDev *lsculldev;
	int ret;
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif
	lsculldev = (struct ScullDev*)&sculldev[0];
	if(!sculldev) {
		printk(KERN_ERR "sculldev not allocated");
		goto ERR;
	}
	
	lsculldev = container_of(inodep->i_cdev,struct ScullDev,c_dev);
	if (!lsculldev) {
		printk(KERN_ERR "container of failed\n");
		goto ERR;	
	}
	
	filep->private_data = lsculldev;
	if((filep->f_flags & O_ACCMODE) == O_WRONLY) {
		#ifdef DEBUG
		printk(KERN_INFO "device write mode\n");
		#endif
		if(down_interruptible(&lsculldev->sem))
			return -ERESTARTSYS;

		ret = trim_dev(lsculldev);
		if(ret < 0) {
			#ifdef DEBUG
			printk(KERN_ERR "device trim failed!\n");
			#endif
			goto SEM_UP;
		}
		up(&lsculldev->sem);
	}
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
SEM_UP:
	up(&lsculldev->sem);
ERR:
	return -1;
}

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

	if (down_interruptible(&lsculldev->sem))
		return -ERESTARTSYS;

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
	data_size = *f_pos;
	filep->f_pos = *f_pos;
	up(&lsculldev->sem);
		
	#ifdef DEBUG
	printk(KERN_INFO "after read,%d bytes present in scull,offset = %d\n",data_size,(int)*f_pos);
	#endif
	
	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	return bytes_read;
ERR:
	up(&lsculldev->sem);
	return -1;
}

loff_t llseek_dev(struct file *filep,loff_t offset,int origin)
{
	loff_t fpos = 0;
	struct ScullDev *lsculldev = filep->private_data;
	#ifdef DEBUG
	printk("START : %s\n",__func__);
	#endif
	if (!lsculldev) {
		printk(KERN_ERR "no device found!\n");
		return -1;
	}

	if (down_interruptible(&lsculldev->sem))
		return -ERESTARTSYS;

	if (offset > device_size) {
		printk(KERN_ERR "offset beyond device_size!\n");
		goto ERR;
	}
	
	switch (origin) {
	case SEEK_SET :
		fpos = offset;
		filep->f_pos = offset;
		break;
	case SEEK_CUR :
		break;
	case SEEK_END :
		break;
	}
	#ifdef DEBUG
	printk(KERN_INFO "fpos = %d\n",(int)fpos);
	#endif
	up(&lsculldev->sem);
	#ifdef DEBUG
	printk("END : %s\n",__func__);
	#endif
	return fpos;
ERR:
	up(&lsculldev->sem);
	return -1;
}

int close_dev(struct inode *inodep,struct file *filep)
{
	#ifdef DEBUG
	printk(KERN_INFO "START: %s \n",__func__);
	#endif

	#ifdef DEBUG
	printk(KERN_INFO "END: %s \n",__func__);
	#endif
	
	return 0;
}
