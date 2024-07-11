/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * File operations for Char Device LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/fs.h>
#include "fops.h"

static bool status = false;
static char dbuf[BUF_LEN] = {0};
static char *dbufp = 0;

/**
 * device_open - device open handler
 *
 * @inode: pointer to inode of the device
 * @file: pointer to file of the device
 *
 * Called when a process opens the device.
 */
static int device_open(struct inode *inode, struct file *file)
{
	static int cnt = 0;
	if (!try_module_get(THIS_MODULE))
		return -ENODEV;
	if (status)
		return -EBUSY;
	status = true;
	++cnt;
	sprintf(dbuf, "[%s] (%s) device opened %d times.\n",
		KBUILD_MODNAME, __func__, cnt);
	pr_info("device opened %d times.", cnt);
	dbufp = dbuf;
	return 0;
}

/**
 * device_release - device release handler
 *
 * @inode: pointer to inode of the device
 * @file: pointer to file of the device
 *
 * Called when a process closes the device.
 */
static int device_release(struct inode *inode, struct file *file)
{
	status = false;
	module_put(THIS_MODULE);
	pr_info("device released.");
	return 0;
}

/**
 * device_read - device read handler
 *
 * @file: pointer to file of the device
 * @buf: pointer to the buffer
 * @count: number of bytes to read
 * @pos: the starting offset
 *
 * Called when a process reads from the device.
 */
static ssize_t device_read(struct file *file, char __user *buf, size_t count,
			   loff_t * pos)
{
	int ret = simple_read_from_buffer(buf, count, pos, dbuf, strlen(dbuf));
	pr_alert("device read %s.", (ret < 0 ? "failed" : "success"));
	return ret;
}

/**
 * device_write - device write handler
 *
 * @file: unused
 * @buf: unused
 * @count: unused
 * @pos: unused
 *
 * Called when a process writes to the device.
 */
static ssize_t device_write(struct file *file, const char __user *buf,
			    size_t count, loff_t * pos)
{
	int ret = simple_write_to_buffer(dbuf, BUF_LEN, pos, buf, count);
	pr_alert("device write %s.", (ret < 0 ? "failed" : "success"));
	return ret;
}

const struct file_operations fops = {
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write,
};