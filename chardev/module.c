/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Char Device Linux Kernel Module
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "module.h"
#include "fops.h"

/**
 * Device related
 */

static dev_t dev_num;
static struct cdev cdev = {
	.owner = THIS_MODULE,
};
static char *device_devnode(const struct device *, umode_t *);
static const struct class class = {
	.name = CLASS_NAME,
	.devnode = device_devnode,
};
static struct device *device = NULL;

/**
 * Function definitions
 */

/**
 * device_node - devnode open handler
 *
 * @dev: pointer to the device struct
 * @mode: mode for the device node
 *
 * Sets mode of the device node to (S_IRUGO | S_IWUGO).
 */
static char *device_devnode(const struct device *dev, umode_t *mode)
{
	if (mode)
		*mode |= S_IRUGO | S_IWUGO;
	return NULL;
}

/**
 * module_init_func - initialize module
 *
 * Initialize the module.
 *
 * Returns 0 if successful.
 */
static int __init module_init_func(void)
{
	int ret;
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (unlikely(ret < 0)) {
		pr_alert("device registeration failed.");
		return ret;
	}
	pr_info("device registered with major %d.", MAJOR(dev_num));
	cdev_init(&cdev, &fops);
	ret = cdev_add(&cdev, dev_num, 1);
	if (unlikely(ret < 0)) {
		pr_alert("device cdev addition failed.");
		unregister_chrdev_region(dev_num, 1);
		return ret;
	}
	ret = class_register(&class);
	if (unlikely(ret < 0)) {
		pr_alert("device class creation failed.");
		cdev_del(&cdev);
		unregister_chrdev_region(dev_num, 1);
		return ret;
	}
	device = device_create(&class, NULL, dev_num, NULL, DEVICE_NAME);
	if (unlikely(IS_ERR(device))) {
		pr_alert("device node creation failed.");
		class_destroy(&class);
		cdev_del(&cdev);
		unregister_chrdev_region(dev_num, 1);
		return PTR_ERR(device);
	}
	pr_info("device node created at /dev/%s.", DEVICE_NAME);
	pr_info("module initialized.");
	return 0;
}

/**
 * module_exit_func - clean up module
 *
 * Clean up on module exit.
 */
static void __exit module_exit_func(void)
{
	device_destroy(&class, dev_num);
	class_destroy(&class);
	cdev_del(&cdev);
	unregister_chrdev_region(dev_num, 1);
	pr_info("device unregistered.");
	pr_info("module exited.");
}

module_init(module_init_func);
module_exit(module_exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pairman Guo");
MODULE_DESCRIPTION("Char Device Linux Kernel Module");
