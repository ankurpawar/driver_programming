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
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/timer.h>
#include<asm/hardirq.h>

#define DEBUG
#define MAJOR_NUM 0
#define MINOR_NUM 0
#define DEV_NAME "Scratch_driver"
#define NOD 1
#define QSET_SIZE 8
#define QUANTUM_SIZE 8
#define DATA_SIZE 0
#define DEVICE_SIZE 4096

#endif
