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

static void *seq_start(struct seq_file *seq, loff_t *pos)
{
	if (*pos > 15) {
		pr_info("start %lld (0).", *pos);
		return NULL;
	}
	pr_info("start %lld.", *pos);
	return pos;
}

static void seq_stop(struct seq_file *seq, void *value)
{
	loff_t *pos = (loff_t *)value;
	pr_info("stop %lld.", (pos ? *pos : 0));
}
/*
[  214.629417] [proc] (proc_open) proc opened 1 times.
[  214.629430] start at 0.
[  214.629431] show 0.
[  214.629432] next 1
[  214.629432] show 1.
[  214.629433] next 2
[  214.629433] show 2.
[  214.629433] next 3
[  214.629434] show 3.
[  214.629434] next 4
[  214.629434] show 4.
[  214.629435] next 5
[  214.629435] show 5.
[  214.629436] next 6
[  214.629436] show 6.
[  214.629436] next 7
[  214.629437] show 7.
[  214.629437] next 8
[  214.629438] show 8.
[  214.629438] next 9
[  214.629438] show 9.
[  214.629439] next 10
[  214.629439] show 10.
[  214.629440] next 11
[  214.629440] show 11.
[  214.629441] next 12
[  214.629441] show 12.
[  214.629441] next 13
[  214.629442] show 13.
[  214.629442] next 14
[  214.629443] show 14.
[  214.629443] next 15
[  214.629443] show 15.
[  214.629444] next 16 (0).
[  214.629446] BUG: kernel NULL pointer dereference, address: 0000000000000000
[  214.629449] #PF: supervisor read access in kernel mode
[  214.629450] #PF: error_code(0x0000) - not-present page
[  214.629451] PGD 0 P4D 0 
[  214.629453] Oops: 0000 [#1] PREEMPT SMP NOPTI
[  214.629455] CPU: 0 PID: 4271 Comm: cat Tainted: G           OE      6.9.7-1.surface.fc40.x86_64 #1
[  214.629457] Hardware name: Microsoft Corporation Surface Pro 7+/Surface Pro 7+, BIOS 25.102.143 04/10/2024
[  214.629458] RIP: 0010:seq_stop+0x9/0x20 [proc]
[  214.629466] Code: cc cc cc 66 66 2e 0f 1f 84 00 00 00 00 00 0f 1f 00 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 f3 0f 1e fa 0f 1f 44 00 00 <48> 8b 36 48 c7 c7 2c e4 21 c2 e9 a8 fe f1 f5 0f 1f 84 00 00 00 00

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