#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>

#define PROC_NAME "readme"

static struct proc_dir_entry *entry=NULL; 

//struct task_struct init_task;

static void *my_seq_start(struct seq_file *seq,loff_t *pos)
{
   struct task_struct *task;	
   loff_t off = 0;


   for_each_process(task)
   {
      if(*pos == off++)   
      {
           printk("in start pos=%d\n",*pos);
	   return task;   
      }	      
  
   }	   

   printk("start seq over\n");
   return NULL;
}


static void *my_seq_next(struct seq_file *seq,void *v,loff_t *pos)
{
    struct task_struct *p=v,*n;	
    n=next_task(p);

    ++*pos;
    printk("in seq_next pos=%d\n",*pos);

    return (n != &init_task)?next_task(n):NULL;    
}

static int my_seq_show(struct seq_file *seq,void *v)
{
   int ret;
   struct task_struct *r,*p;
   int num_thread=0;
   r=v;
   p=v;   


   do{
       num_thread++;
   }while_each_thread(p,r);
      
   ret=seq_printf(seq,"name = %s , pid = %d , tgid = %d , stack addr = %d , num threads = %d \n"
		         ,p->comm,p->pid,p->tgid,p->stack+THREAD_SIZE,num_thread);

   printk("seq_printf returned %d",ret);
   
   return 0;
}


static void my_seq_stop(struct seq_file *seq,void *v)
{
   printk("in stop function\n");
}


static struct seq_operations my_seq_ops = {
    .start = my_seq_start,
    .next  = my_seq_next,
    .show  = my_seq_show,
    .stop  = my_seq_stop 
};

static int my_seq_open(struct inode *inode,struct file *file)
{
    printk("in my_seq_open\n");
    return seq_open(file,&my_seq_ops); 
}

static struct file_operations my_file_ops = {
    .owner = THIS_MODULE ,
    .open  = my_seq_open ,
    .read  = seq_read ,
    .llseek = seq_lseek ,  
    .release = seq_release
};

static int __init procex_init(void)
{
   entry = create_proc_entry(PROC_NAME,S_IRUSR,NULL);
   if(entry)
   {
      entry->proc_fops = &my_file_ops;
   }
   else
   {
      return -EINVAL;
   }	   

   return 0;
}


static void procex_exit(void)
{
   	
   remove_proc_entry(PROC_NAME,NULL);
   printk("exiting procex module\n"); 
}

module_init(procex_init);
module_exit(procex_exit);
