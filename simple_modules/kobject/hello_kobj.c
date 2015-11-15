/*
 * Kobject example module for linux.
 *
 * Copyright (C) 2015 Ankur Pawar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kobject.h>


MODULE_ALIAS("hi_Kobj");
MODULE_DESCRIPTION("hello world with kobject");
MODULE_AUTHOR("Ankur");
MODULE_LICENSE("GPL");

static int eg;
static struct kobject *lkobj;

static ssize_t eg_show(struct kobject *kobj, struct kobj_attribute *attr,
		       char *buf)
{
	dump_stack();
	return sprintf(buf, "%d\n", eg);
}

static ssize_t eg_store(struct kobject *kobj, struct kobj_attribute *attr,
		        const char *buf, size_t count)
{
	dump_stack();
	sscanf(buf, "%du", &eg);
	return count;
}

static struct kobj_attribute eg_attr = 
	__ATTR(eg, 0666, eg_show, eg_store);
static struct attribute *attr[] = {
	&eg_attr.attr,
	NULL,
};


static int __init initialization(void)
{
	int ret = 0;
	printk(KERN_INFO "hello world module\n");
	lkobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (lkobj == NULL)
		return ENOMEM;
	ret = sysfs_create_file(lkobj, &eg_attr.attr);
	if (ret)
		kobject_put(lkobj);
	return ret;
}
module_init(initialization);

static void __exit cleanup(void)
{
	printk(KERN_INFO "good bye world\n");
	kobject_put(lkobj);
}
module_exit(cleanup); 
