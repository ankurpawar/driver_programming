#include"header.h"
#include"declarations.h"
#include"ioctl_dev.h"

long ioctl_dev(struct file *filep, unsigned int cmd, unsigned long arg)
{
	long retval = 0;
	int err = 0;
	
	#ifdef DEBUG
	printk(KERN_INFO "START : %s \n",__func__);
	#endif
	
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) {
		printk(KERN_ERR "magic number not matching\n");
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) {
		printk(KERN_ERR "bits exceeding amx numbe of bits\n");
		return -ENOTTY;
	}
	
	if (_IOC_DIR(cmd) & _IOC_READ) {
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	} else if (_IOC_DIR(cmd) & _IOC_WRITE) {
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}
	if (err) {
		printk(KERN_ERR "access error\n");
		return -EFAULT;
	}
	#ifdef DEBUG
	printk(KERN_INFO "cmd = %u\n",cmd);
	#endif
	
	switch (cmd) {

	case SCULL_IOCSQUANTUM:
		if (! capable(CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(quantum_size,(int __user*)arg);
		break;

	case SCULL_IOCSQSET:
		if (! capable(CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(qset_size,(int __user*)arg);
		break;

 	case SCULL_IOCGQUANTUM:
		retval = __put_user(quantum_size,(int __user*)arg);
		break;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	
	#ifdef DEBUG
	printk("END : %s \n",__func__);
	#endif

	return retval;
}

