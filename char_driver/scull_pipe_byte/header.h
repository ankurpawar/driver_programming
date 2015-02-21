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

#ifndef DEBUG
#define DEBUG
#endif

#ifndef MAJOR_NUM
#define MAJOR_NUM 0
#endif

#ifndef MINOR_NUM
#define MINOR_NUM 0
#endif

#ifndef DEV_NAME
#define DEV_NAME "scull_pipe"
#endif

#ifndef NOD
#define NOD 1
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif

#endif
