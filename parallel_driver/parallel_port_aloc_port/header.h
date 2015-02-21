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
#define DEV_NAME "my_driver"
#endif

#ifndef NOD
#define NOD 1
#endif

#ifndef QSET_SIZE
#define QSET_SIZE 3
#endif

#ifndef QUANTUM_SIZE
#define QUANTUM_SIZE 1
#endif

#ifndef DATA_SIZE
#define DATA_SIZE 0
#endif

#ifndef DEVICE_SIZE
#define DEVICE_SIZE 1
#endif

#ifndef PORT_ADDRESS
#define	PORT_ADDRESS 0x378
#endif

#ifndef DATA_PORT
#define DATA_PORT PORT_ADDRESS
#endif

#ifndef STATUS_PORT
#define STATUS_PORT 0x379
#endif

#ifndef CONTROL_PORT
#define CONTROL_PORT 0x37A
#endif

#ifndef NUM_ADDRESS
#define	NUM_ADDRESS 3
#endif

#endif
