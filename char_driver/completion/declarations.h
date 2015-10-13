#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct ScullDev
{
	struct ScullQset *qset;
	struct cdev c_dev;
	int qset_size;
	int quantum_size;
	int device_size;
	int data_size;
	struct completion my_completion;
};

struct ScullQset
{
	struct ScullQset *next;
	void **data;
};

extern int major_num;
extern unsigned int minor_num;
extern unsigned int num_dev;
extern int qset_size;
extern int quantum_size;	
extern int data_size;
extern int device_size;
extern dev_t dev;
extern struct ScullDev *sculldev;
extern struct file_operations fops;

int open_dev(struct inode *,struct file *);
int close_dev(struct inode *,struct file *);
int trim_dev(struct ScullDev *);
ssize_t write_dev(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_dev(struct file *,char __user *,size_t count,loff_t *);
loff_t llseek_dev(struct file *,loff_t,int);
struct ScullQset *create_scull(int count);
struct ScullQset *create_scullqset(int count);
int create_qset(int count,struct ScullQset*);
int create_quantums(int count,struct ScullQset*);
struct ScullQset *scull_follow(struct ScullQset *,int);
#endif
