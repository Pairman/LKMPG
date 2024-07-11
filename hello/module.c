/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Hello World Linux Kernel Module
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include "module.h"

static struct params_t params = {
	.b = false,
	.ia_len = 16,
	.ia = {0},
	.cp = "",
};

module_param_named(b, params.b, bool, PARAM_MODE);
MODULE_PARM_DESC(b, " (default false)");
module_param_array_named(ia, params.ia, int, &params.ia_len, PARAM_MODE);
MODULE_PARM_DESC(ia, " (default {0})");
module_param_named(cp, params.cp, charp, PARAM_MODE);
MODULE_PARM_DESC(cp, " (default \"\")");

/**
 * module_init_func - initialize module
 *
 * Initialize the module.
 *
 * Returns 0 if successful.
 */
static int __init module_init_func(void)
{
	int i;
	pr_info("hello world.\n");
	pr_info("param b: %s\n", (params.b ? "true" : "false"));
	pr_info("param ia[%d]: {", params.ia_len);
	pr_cont("%d", params.ia[0]);
	for (i = 1; i < params.ia_len; ++i)
		pr_cont(", %d", params.ia[i]);
	pr_cont("}\n");
	pr_info("param cp: \"%s\"\n", params.cp);
	return 0;
}

/**
 * module_exit_func - clean up module
 *
 * Called on module exit.
 */
static void __exit module_exit_func(void)
{
	pr_info("bye world.\n");
}

module_init(module_init_func);
module_exit(module_exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pairman Guo");
MODULE_DESCRIPTION("Hello World Linux Kernel Module");
