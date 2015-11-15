#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>

#define PROC_NAME "readme"

static struct proc_dir_entry *proc_entry = NULL; 

static void *my_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct task_struct *task;
	loff_t off = 0;

	for_each_process (task) {
		if(*pos == off++) {
			printk("in start pos=%u\n", (unsigned int)off);
			return task;   
		}
	}
	printk("start seq over\n");
	return NULL;
}

static void *my_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct task_struct *p=v;	
	struct task_struct *n;
	n = next_task(p);
	
	++*pos;
	printk("in seq_next pos=%u\n",(unsigned int)*pos);
	return (n != &init_task) ? next_task(n) : NULL;    
}

static int my_seq_show(struct seq_file *seq, void *v)
{
	int ret;
	struct task_struct *p;
	struct task_struct *r;
	int num_thread = 0;
	r = (struct task_struct*) v;
	p = (struct task_struct*) v;
	
	do {
		num_thread++;
	} while_each_thread(p, r);

	ret = seq_printf(seq, "name = %s," 
				"pid = %d ," 
				"tgid = %d ,"
				"stack addr = %d ," 
				"num threads = %d \n",
				p->comm,
				p->pid,
				p->tgid,
				p->stack+THREAD_SIZE,
				num_thread);

	printk("seq_printf returned %d", ret);
	return 0;
}

static void my_seq_stop(struct seq_file *seq, void *v)
{
	printk("in stop function\n");
}

static struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next  = my_seq_next,
	.show  = my_seq_show,
	.stop  = my_seq_stop
};

static int my_seq_open(struct inode *inode, struct file *file)
{
	printk("in my_seq_open\n");
	return seq_open(file, &my_seq_ops); 
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = my_seq_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

static int __init procex_init(void)
{
	proc_entry = proc_create(PROC_NAME, 0, NULL, &fops);
	if (!proc_entry) {
		printk(KERN_ERR "error in creating proc entry\n");
		goto ERR;
	}
	return 0;
ERR:
	return -1;
}

static void __exit procex_exit(void)
{  	
   remove_proc_entry(PROC_NAME, NULL);
   printk("exiting procex module\n");
}

module_init(procex_init);
module_exit(procex_exit);
