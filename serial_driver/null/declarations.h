#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct serial_dev
{
	struct cdev c_dev;
	struct semaphore sem;
};

extern int major_num;
extern unsigned int minor_num;
extern unsigned int num_dev;
extern dev_t dev;
extern struct serial_dev *serialdev;
extern struct file_operations fops;
extern unsigned long port_address;
extern int num_regs;
extern struct resource *serial_port;

int open_dev(struct inode *,struct file *);
int close_dev(struct inode *,struct file *);
ssize_t write_dev(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_dev(struct file *,char __user *,size_t count,loff_t *);
loff_t llseek_dev(struct file *,loff_t,int);
#endif
