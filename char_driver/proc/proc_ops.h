#ifndef PROC_OPS_H
#define PROC_OPS_H

#include<linux/proc_fs.h>
#include<linux/seq_file.h>

void *scull_seq_start(struct seq_file *, loff_t *);
void *scull_seq_next(struct seq_file *, void *, loff_t *);
void scull_seq_stop(struct seq_file *, void *);
int scull_seq_show(struct seq_file *, void *);

int scull_proc_open(struct inode*,struct file *);

#endif
