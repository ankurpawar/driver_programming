#ifndef BLOCK_DRIVER_H
#define BLOCK_DRIVER_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/param.h>
#include <linux/hdreg.h>
 
#define MAJOR_NUM 0
#define NUM_DEV 1
#define SECTOR_SIZE 512
#define NUM_SECTOR 1024
#define DEV_NAME "sbd"
#define SBULL_MINORS 4
#define KERNEL_SECTOR_SIZE 512

#define INVALIDATE_DELAY 30*HZ

struct sbull_dev {
	int size;
	unsigned char *data;
	spinlock_t lock;
	struct request_queue *queue;
	struct gendisk *gd;
	int users;
	struct timer_list timer;
	short media_change;
};

#endif
