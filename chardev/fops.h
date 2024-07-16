/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Header for file operations for Char Device LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#ifndef _CHARDEV_FOPS_H
#define _CHARDEV_FOPS_H

#include <linux/fs.h>
#include <linux/ioctl.h>

#define BUF_LEN 512

#define IOCTL_MAGIC 'f'
#define IOCTL_DBUF_GET _IOR(IOCTL_MAGIC, 'f', char *)
#define IOCTL_DBUF_SET _IOR(IOCTL_MAGIC, 'g', char *)
#define IOCTL_DBUF_GETN _IOR(IOCTL_MAGIC, 'h', int)
#define IOCTL_DBUF_SETN _IOW(IOCTL_MAGIC, 'i', int)

/**
 * Variable prototypes
 */

extern const struct file_operations fops;

#endif /* _CHARDEV_FOPS_H */
