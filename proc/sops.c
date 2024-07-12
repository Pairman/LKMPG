/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Seq operations for Procfile LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#define pr_fmt(fmt) "[%s] (%s) " fmt, KBUILD_MODNAME, __func__

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "sops.h"

/**
 * seq_start - seq start handler
 *
 * @seq: unused
 * @pos: pointer to the starting offset
 *
 * Called when a process starts seq operations on the file.
 */
static void *seq_start(struct seq_file *seq, loff_t *pos)
{
	if (*pos > 15) {
		pr_info("start %lld (0).", *pos);
		return NULL;
	}
	pr_info("start %lld.", *pos);
	return pos;
}

/**
 * seq_stop - seq stop handler
 *
 * @seq: unused
 * @value: pointer to the value
 *
 * Called when a process stops seq operations on the file.
 */
static void seq_stop(struct seq_file *seq, void *value)
{
	loff_t *pos = (loff_t *)value;
	pr_info("stop %lld.", (pos ? *pos : 0));
}

/**
 * seq_next - seq next handler
 *
 * @seq: unused
 * @value: pointer to the value
 * @pos: pointer to the current offset
 *
 * Called when a process continues seq operations on the file.
 */
static void *seq_next(struct seq_file *seq, void *value, loff_t *pos)
{
	if (++(*pos) > 15) {
		pr_info("next %lld (0).", *pos);
		return NULL;
	}
	pr_info("next %lld", *pos);
	return pos;
}

/**
 * seq_show - seq show handler
 *
 * @seq: pointer to the seq file
 * @value: pointer to the value
 *
 * Called when a process seq shows on the file.
 */
static int seq_show(struct seq_file *seq, void *value)
{
	loff_t *pos = (loff_t *)value;
	pr_info("show %lld.", *pos);
	seq_printf(seq, "%lld\n", *pos);
	return 0;
}

const struct seq_operations sops = {
	.start = seq_start,
	.stop = seq_stop,
	.next = seq_next,
	.show = seq_show,
};