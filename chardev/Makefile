# SPDX-License-Identifier: GPL-3.0-only
#
# Makefile for the Char Device LKM
#

MNAME := chardev
MDIR := $(if $(PWD),$(PWD),.)
KDIR := /lib/modules/$(shell uname -r)/build

obj-m += $(MNAME).o
$(MNAME)-y := module.o fops.o

.PHONY: all clean insmod modinfo rmmod dmesg

all:
	$(MAKE) -C $(KDIR) M=$(MDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(MDIR) clean

insmod:
	sudo insmod $(MDIR)/$(MNAME).ko

modinfo:
	modinfo $(MDIR)/$(MNAME).ko

rmmod:
	sudo rmmod $(MNAME)

dmesg:
	sudo dmesg | grep -E "^\[\s+?[0-9]+\.[0-9]+\] \[$(MNAME)\]" | tail -24