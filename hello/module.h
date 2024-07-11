/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Header file for Hello World LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#ifndef _HELLO_MODULE_H
#define _HELLO_MODULE_H

#include <linux/fcntl.h>

#define PARAM_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

struct params_t {
	bool b;
	int ia_len;
	int ia[16];
	char *cp;
};

#endif /* _HELLO_MODULE_H */
