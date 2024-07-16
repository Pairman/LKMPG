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

static bool dstat = false;
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
	if (dstat)
		return -EBUSY;
	dstat = true;
	++cnt;
	sprintf(dbuf, "[%s] (%s) device opened %d times.",
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
	dstat = false;
	module_put(THIS_MODULE);
	pr_info("device released.");
	return 0;
}

/**
 * device_read - device read handler
 *
 * @file: unused
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
 * @buf: pointer to the buffer
 * @count: number of bytes to read
 * @pos: the starting offset
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

/**
 * device_write - device unlocked ioctl handler
 *
 * @file: unused
 * @buf: unused
 * @count: unused
 * @pos: unused
 *
 * Called when a process does ioctl with the device.
 */
static long device_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	char __user *buf = (char __user *)arg;
	char c;
	loff_t pos = 0;
	switch (cmd) {
	case IOCTL_DBUF_GET:
		if (device_read(file, buf, BUF_LEN, &pos) < 0) {
			pr_alert("device ioctl get failed.");
			return -EFAULT;
		}
		pr_info("device ioctl get got \"%s\".", dbuf);
		break;
	case IOCTL_DBUF_SET:
		if (device_write(file, buf, BUF_LEN, &pos) < 0) {
			pr_alert("device ioctl set failed.");
			return -EFAULT;
		}
		pr_info("device ioctl set set to \"%s\".", dbuf);
		break;
	case IOCTL_DBUF_GETN:
		if (arg >= BUF_LEN) {
			pr_alert("device ioctl getn invalid index %ld.", arg);
			return -EINVAL;
		}
		c = dbuf[arg];
		pr_info("device ioctl getn got '%c' at index %ld.", c, arg);
		return (long)c;
		break;
	case IOCTL_DBUF_SETN:
		if (arg >= BUF_LEN) {
			pr_alert("device ioctl setn invalid index %ld.", arg);
		}
		dbuf[arg] = (char)(arg % 94 + 33);
		pr_info("device ioctl setn set to '%c' at %ld.", dbuf[arg], arg);
		break;
	default:
		return -ENOTTY;
	}
	return 0;
}

const struct file_operations fops = {
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_unlocked_ioctl,
};