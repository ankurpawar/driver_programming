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
#define DEV_NAME "Scratch_driver"
#endif

#ifndef NOD
#define NOD 1
#endif

#ifndef QSET_SIZE
#define QSET_SIZE 8
#endif

#ifndef QUANTUM_SIZE
#define QUANTUM_SIZE 8
#endif

#ifndef DATA_SIZE
#define DATA_SIZE 0
#endif

#ifndef DEVICE_SIZE
#define DEVICE_SIZE 4096
#endif

#endif
