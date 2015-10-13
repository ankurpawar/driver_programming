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
#include<linux/sched.h>
#include<linux/wait.h>

#define DEBUG
#define MAJOR_NUM 0
#define MINOR_NUM 0
#define DEV_NAME "scull_pipe"
#define NOD 1
#define MAX_BUFFER_SIZE 4096

#endif
