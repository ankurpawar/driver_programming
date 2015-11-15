#ifndef HEADER_H
#define HEADER_H

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/cdev.h>
#include<linux/moduleparam.h>
#include<linux/stat.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>

#define DEBUG
#define MAJOR_NUM 0
#define MINOR_NUM 0
#define DEV_NAME "Scratch_driver"
#define NOD 1
#define QSET_SIZE 8
#define QUANTUM_SIZE 8
#define DATA_SIZE 0
#define DEVICE_SIZE 4096

struct ScullDev
{
	struct ScullQset *qset;
	struct cdev c_dev;
	int qset_size;
	int quantum_size;
	int device_size;
	int data_size;
	struct semaphore sem;
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
extern struct seq_operations scull_seq_ops;
extern struct file_operations scull_proc_ops;

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
