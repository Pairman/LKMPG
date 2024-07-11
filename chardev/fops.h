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

#define BUF_LEN 512

/**
 * Variable prototypes
 */

extern struct file_operations fops;

#endif /* _CHARDEV_FOPS_H */
