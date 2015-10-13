#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct ScullPipe
{
	wait_queue_head_t inq,outq;
	char *buffer,*end;
	char *rp,*wp;
	struct cdev c_dev;
	int buffer_size;
	struct semaphore sem;
};

extern int major_num;
extern unsigned int minor_num;
extern unsigned int num_dev;
extern dev_t dev;
extern struct ScullPipe *scullpipe;
extern struct file_operations fops;
extern int max_pipe_size;

int open_pipe(struct inode *,struct file *);
int close_pipe(struct inode *,struct file *);
int trim_pipe(struct ScullPipe *);
ssize_t write_pipe(struct file *,const char __user *,size_t count,loff_t *);
ssize_t read_pipe(struct file *,char __user *,size_t count,loff_t *);
char *create_pipe(int count);
#endif
