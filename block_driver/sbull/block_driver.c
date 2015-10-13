#include "block_driver.h"
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int major_num;
static int num_dev;
static int sect_size;
static int nsector;
static struct sbull_dev *devices = NULL;


void set_defaults(void)
{
	major_num = MAJOR_NUM;
	num_dev = NUM_DEV;
	sect_size = SECTOR_SIZE;
	nsector = NUM_SECTOR;
}

int sbull_media_changed(struct gendisk *gd)
{
	struct sbull_dev *dev = gd->private_data;
	
	return dev->media_change;
}

static void sbull_transfer(struct sbull_dev *dev, unsigned long sector, 
			unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector * KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect * KERNEL_SECTOR_SIZE;
	
	if ((offset + nbytes) > dev->size) {
		printk(KERN_NOTICE "write beyond the end %ld %ld \n", offset, nbytes);
		return;
	}
	if (write)
		memcpy(dev->data + offset,buffer, nbytes);
	else
		memcpy(buffer, dev->data + offset , nbytes);
}

static void sbull_request(struct request_queue *q)
{
	struct request *req;
	req = blk_fetch_request(q);
	while (req != NULL) {
		struct sbull_dev *dev = req->rq_disk->private_data;
		if (req->cmd_type != REQ_TYPE_FS) {
			printk(KERN_NOTICE "skip non fs request\n");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		sbull_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req), 
				req->buffer, rq_data_dir(req));
		if ( ! __blk_end_request_cur(req, 0)) {
			req = blk_fetch_request(q);
		}
	}
}

static int sbull_open(struct block_device *device, fmode_t mode)
{
	struct sbull_dev *dev = device->bd_disk->private_data;
	
	del_timer_sync(&dev->timer);
	spin_lock(&dev->lock);
	if (! dev->users)
		check_disk_change(device->bd_inode->i_bdev);
	dev->users++;
	spin_unlock(&dev->lock);
	return 0;
}

static void sbull_release(struct gendisk *disk, fmode_t mode)
{
	struct sbull_dev *dev = disk->private_data;
	
	spin_lock(&dev->lock);
	dev->users--;
	if( !dev->users) {
		dev->timer.expires = jiffies + INVALIDATE_DELAY;
		add_timer(&dev->timer);
	}
	spin_unlock(&dev->lock);
}

void sbull_invalidate(unsigned long ldev)
{
	struct sbull_dev *dev = (struct sbull_dev *) ldev;

	spin_lock(&dev->lock);
	if (dev->users || !dev->data) 
		printk (KERN_WARNING "sbull: timer sanity check failed\n");
	else
		dev->media_change = 1;
	spin_unlock(&dev->lock);
}

int sbull_revalidate(struct gendisk *gd)
{
	struct sbull_dev *dev = gd->private_data;
	
	if (dev->media_change) {
		dev->media_change = 0;
		memset (dev->data, 0, dev->size);
	}
	return 0;
}

int sbull_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	/* We have no real geometry, of course, so make something up. */
	geo->cylinders = get_capacity(bdev->bd_disk) / (4 * 16);
	geo->heads = 4;
	geo->sectors = 16;
	geo->start = 0;
	return 0;
}


static struct block_device_operations sbull_ops = {
	.owner           = THIS_MODULE,
	.open 	         = sbull_open,
	.release 	 = sbull_release,
	.media_changed   = sbull_media_changed,
	.revalidate_disk = sbull_revalidate,
	.getgeo 	 = sbull_getgeo, 
};

void setup_device(struct sbull_dev *dev, int nth)
{
	memset(dev , 0, sizeof (struct sbull_dev));
	dev->size = nsector * sect_size;
	dev->data = vmalloc(dev->size);
	if (dev->data == NULL) {
		printk(KERN_WARNING "vmalloc failed\n");
		return;
	}
	spin_lock_init(&dev->lock);
	init_timer(&dev->timer);
	dev->timer.data = (unsigned long)dev;
	dev->timer.function = sbull_invalidate;

	dev->queue = blk_init_queue(sbull_request, &dev->lock);
	if (dev->queue == NULL) {
		printk(KERN_WARNING "error in init queue\n");
		goto err_vfree;
	}
	blk_queue_logical_block_size(dev->queue, sect_size);
	dev->queue->queuedata = dev;
	
	/*add gendisk struct*/
	dev->gd = alloc_disk(SBULL_MINORS);
	if (! dev->gd) {
		printk(KERN_NOTICE "alloc_disk failed\n");
		goto err_vfree;
	}
	dev->gd->major = major_num;
	dev->gd->first_minor = nth * SBULL_MINORS;
	dev->gd->fops = &sbull_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	snprintf (dev->gd->disk_name, 32, "sbull%c", nth + 'a');
	set_capacity(dev->gd, nsector*(sect_size/KERNEL_SECTOR_SIZE));
	add_disk(dev->gd);
	return;

err_vfree:
	if(dev->data)
		vfree(dev->data);
}

static int __init init_block_driver(void)
{
	int i;
	printk(KERN_INFO "START: block device %s\n",__func__);
	set_defaults();
	/*
	 *register block device
	 */
	major_num = register_blkdev(major_num, DEV_NAME);
	if (major_num <= 0) {
		printk(KERN_WARNING "error in registering block device\n");
		return -EBUSY;
	}
	/*
	 *allocate devices
	 */
	devices = kmalloc(num_dev * sizeof(struct sbull_dev), GFP_KERNEL);
	if (devices == NULL) {
		printk(KERN_WARNING "error in allocating memory\n");
		goto err_unregister;
	}
	for (i = 0; i < num_dev; i++) 
		setup_device(devices + i, i);

	printk(KERN_INFO "END: block device %s\n",__func__);
	return 0;

err_unregister:
	unregister_blkdev(major_num, DEV_NAME);
	return -ENOMEM;
}
module_init(init_block_driver);

static void exit_block_driver(void)
{
	int i;
	printk(KERN_INFO "START: block device %s\n",__func__);
	for (i = 0; i < num_dev; i++) {
		struct sbull_dev *dev = devices + i;

		del_timer_sync(&dev->timer);
		if (dev->gd) {
			del_gendisk(dev->gd);
			put_disk(dev->gd);
		}
		if (dev->queue) {
			/*if (request_mode == RM_NOQUEUE)
				blk_put_queue(dev->queue);
			else*/
				blk_cleanup_queue(dev->queue);
		}
		if (dev->data)
			vfree(dev->data);
	}
	unregister_blkdev(major_num, DEV_NAME);
	printk(KERN_INFO "END: block device %s\n",__func__);
}
module_exit(exit_block_driver);
