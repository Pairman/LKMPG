/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Proc operations for Procfile LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include "pops.h"
#ifdef _USE_SEQ_OPS
#include "sops.h"
#endif /* _USE_SEQ_OPS */

#ifndef _USE_SEQ_OPS
static char pbuf[BUF_LEN] = {0};
static char *pbufp = 0;
#endif /* _USE_SEQ_OPS */

#ifdef _USE_SEQ_OPS

/**
 * proc_open - proc open handler
 *
 * @inode: unused
 * @file: pointer to the proc file
 *
 * Called when a process opens the proc file. Based on seq operations.
 */
static int proc_open(struct inode *inode, struct file *file)
{
	static int cnt = 0;
	++cnt;
	pr_info("proc opened %d times.", cnt);
	return seq_open(file, &sops);
}

#else /* _USE_SEQ_OPS */

/**
 * proc_open - proc open handler
 *
 * @inode: pointer to inode of the proc file
 * @file: pointer to the proc file
 *
 * Called when a process opens the proc file.
 */
static int proc_open(struct inode *inode, struct file *file)
{
	static int cnt = 0;
	++cnt;
	sprintf(pbuf, "[%s] (%s) proc opened %d times.\n",
		KBUILD_MODNAME, __func__, cnt);
	pbufp = pbuf;
	pr_info("proc opened %d times.", cnt);
	return 0;
}

/**
 * proc_release - proc release handler
 *
 * @inode: pointer to inode of the proc file
 * @file: pointer to the proc file
 *
 * Called when a process closes the proc file.
 */
static int proc_release(struct inode *inode, struct file *file)
{
	pr_info("proc released.");
	return 0;
}

/**
 * proc_read - proc read handler
 *
 * @file: pointer to the proc file
 * @buf: pointer to the buffer
 * @count: number of bytes to read
 * @pos: pointer to the starting offset
 *
 * Called when a process reads from the proc file.
 */
static ssize_t proc_read(struct file *file, char __user *buf, size_t count,
			 loff_t *pos)
{
	int ret = simple_read_from_buffer(buf, count, pos, pbuf, strlen(pbuf));
	pr_alert("proc read %s.", (ret < 0 ? "failed" : "success"));
	return ret;
}

/**
 * proc_write - proc write handler
 *
 * @file: unused
 * @buf: pointer to the buffer
 * @count: number of bytes to read
 * @pos: pointer to the starting offset
 *
 * Called when a process writes to the proc file.
 */
static ssize_t proc_write(struct file *file, const char __user *buf,
			  size_t count, loff_t *pos)
{
	int ret = simple_write_to_buffer(pbuf, BUF_LEN, pos, buf, count);
	pr_alert("proc write %s.", (ret < 0 ? "failed" : "success"));
	return ret;
}

#endif /* _USE_SEQ_OPS */

const struct proc_ops pops = {
	.proc_open = proc_open,
#ifdef _USE_SEQ_OPS
	.proc_release = seq_release,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
#else /* _USE_SEQ_OPS */
	.proc_release = proc_release,
	.proc_read = proc_read,
	.proc_write = proc_write,
#endif /* _USE_SEQ_OPS */
};