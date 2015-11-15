/*
 * Linux kernel module for simple USB hot plugging.
 *
 * Copyright (C) 2015 Ankur Pawar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/usb.h>

#define HP_V220W_VENDOR_ID	0x03f0  
#define HP_V220W_PRODUCT_ID	0x5a07  

static struct usb_device_id
hpv220w_table [] = {
	{ USB_DEVICE(HP_V220W_VENDOR_ID, HP_V220W_PRODUCT_ID) },
	{ } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, hpv220w_table);

static int __init initialization(void)
{
	printk(KERN_INFO "hello world module\n");
	return 0;
}
module_init(initialization);

static void __exit cleanup(void)
{
	printk(KERN_INFO "good bye world\n");
}
module_exit(cleanup); 

MODULE_LICENSE("GPL");
