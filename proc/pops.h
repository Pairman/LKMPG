/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Header for proc operations for Procfile LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#ifndef _PROC_PROC_H
#define _PROC_PROC_H

#include <linux/proc_fs.h>

#define PROC_NAME "procfile"
#define BUF_LEN 512

/**
 * Variable prototypes
 */

extern const struct proc_ops pops;

#endif /* _PROC_PROC_H */