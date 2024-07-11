/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Procfile Linux Kernel Module
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include "module.h"
#include "pops.h"

/**
 * module_init_func - initialize module
 *
 * Initialize the module.
 *
 * Returns 0 if successful.
 */
static int __init module_init_func(void)
{
	struct proc_dir_entry *proc;
	proc = proc_create(PROC_NAME, S_IRUGO | S_IWUGO, NULL, &pops);
	if (!proc) {
		pr_alert("proc file creation failed.");
		return -EINVAL;
	}
#ifdef _USE_SEQ_OPS
	pr_info("proc file created with seq ops.");
#else
	pr_info("proc file created.\n");
#endif /* _USE_SEQ_OPS */
	pr_info("module initialized.\n");
	return 0;
}

/**
 * module_exit_func - clean up module
 *
 * Called on module exit.
 */
static void __exit module_exit_func(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_info("proc file removed.\n");
	pr_info("module exited.\n");
}

module_init(module_init_func);
module_exit(module_exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pairman Guo");
MODULE_DESCRIPTION("Procfile Linux Kernel Module");
