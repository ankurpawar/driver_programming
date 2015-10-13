#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct parallel_dev
{
	struct cdev c_dev;
	int device_size;
	int data_size;
	struct semaphore sem;
};

extern int major_num;
extern unsigned int minor_num;
extern unsigned int num_dev;
extern int data_size;
extern int device_size;
extern dev_t dev;
extern struct parallel_dev *par_dev;
extern struct file_operations fops;
extern unsigned long port_address;
extern int num_address;

int open_dev(struct inode *,struct file *);
int close_dev(struct inode *,struct file *);
ssize_t write_dev(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_dev(struct file *,char __user *,size_t count,loff_t *);
#endif
