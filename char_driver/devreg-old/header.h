#ifndef HEADER_H
#define HEADER_H

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>

MODULE_LICENSE("GPL");

extern int major_num;
#define DEV_NAME "my_devices"

#endif
