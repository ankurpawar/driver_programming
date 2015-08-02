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
#include<linux/ioport.h>
#include<linux/bitops.h>

#define DEBUG
#define MAJOR_NUM 0
#define MINOR_NUM 0
#define DEV_NAME "my_driver"
#define NOD 1
#define DATA_SIZE 0
#define DEVICE_SIZE 1
#define	PORT_ADDRESS 0x378
#define DATA_PORT PORT_ADDRESS
#define STATUS_PORT 0x379
#define CONTROL_PORT 0x37A
#define	NUM_ADDRESS 3

#endif
